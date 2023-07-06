#include <Keyboard.h>
#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal.h>
//#include <cmath>

///////////////////////////////Analog PINS///////////////////////////////
const int pressurePin = A0; // Analog pin connected to the Pressure sensor
const int TempIn = A3; // Analog pin connected to the Temp sensor

///////////////////////////////Digital PINS///////////////////////////////
const int BUTTON_PIN = 13; //left one
const int OPTIONS_PIN = 1;
#define START_CALIB_PIN 13 // Start calibration
#define RESET_CALIB_PIN 1 // Reset calibration
//////////////////////////////KETBOARD//////////////////////////////
const int joystickX = A1;
const int joystickY = A2;
const int joystickSW = 0;
const int buttonSPin = 10;  // Pin for 's' button  - X
const int buttonAPin = 7;  // Pin for 'a' button -SQUARE
const int buttonDPin = 9;  // Pin for 'd' button -CIRCLE
const int buttonWPin = 8;  // Pin for 'w' button  -TRIAGLE

///////////////////////////////CONFIG///////////////////////////////
int MaxLungsThreshold=0;
int MinLungsThreshold=0;
float D1 = 0.005; //ventiury diameter 1
float D2 = 0.001; //ventiury diameter 2
float alpha = 0.5; // Filter coefficient (0 < alpha < 1)
float density = 0.001295;
float BIAS = 2.13;
float CALIB_INTEGRAL_THRESHOLD = 0.08 ; //Threshold for the lungs volume integral while calibration
float PLAIN_INTEGRAL_THRESHOLD = 0.08; //Threshold for the lungs volume integral whil excersize
int CALIB_TIME_TH = 10;
int GAME_TIME_TH = 15;
float BETA = 0.4; // stager config parameter between 0 to 2
float GAMA = 1;

////////////////////////////Screen Setup///////////////////////////////
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

///////////////////////////////Functions///////////////////////////////
static void PushX(int xVal) //Func who receive the X data and press right/left if it pass the thresholds
{
  if (xVal < 300)
    {
        Keyboard.press(KEY_DOWN_ARROW);
    }
    else if (xVal > 700)
    {
        Keyboard.press(KEY_UP_ARROW);
    }
    else
    {
        Keyboard.release(KEY_UP_ARROW);
        Keyboard.release(KEY_DOWN_ARROW);
    }
    //delay(60);

}
static void PushY(int yVal) //Func who receive the Y data and press down/up if it pass the thresholds
{
    // Map Y axis to arrow keys       
    if (yVal < 300)
    {
        Keyboard.press(KEY_RIGHT_ARROW);
    }
    else if (yVal > 700)
    {
        Keyboard.press(KEY_LEFT_ARROW);
    }
    else
    {
        Keyboard.release(KEY_LEFT_ARROW);
        Keyboard.release(KEY_RIGHT_ARROW);
    }
    //delay(60);
}

static void PushSpace(int swVal) //Func who receive the SW data and press space if 0, else-nothing
{
  
    // Map switch to spacebar
    if (swVal == LOW)
    {
        Keyboard.press(' ');
    }
    else
    {
        Keyboard.release(' ');
    }
}

static void PushEnter(void)
{
    Keyboard.press(KEY_RETURN); // Press ENTER
    delay(100);
}

static void FullControler(void)
{
    int xVal = analogRead(joystickX);
    int yVal = analogRead(joystickY);
    int swVal = digitalRead(joystickSW);
    // Map X axis to arrow keys
    PushX(xVal);
    // Map Y axis to arrow keys
    PushY(yVal);
    // Map switch to spacebar
    PushSpace(swVal);
    // Check if 's' button is pressed
    // Blue s
    if (digitalRead(buttonSPin) == LOW) {
      Keyboard.press('i');
      delay(40);
      Keyboard.release('i');
      }
    // Check if 'options' button was pushed
    //
    if (digitalRead(OPTIONS_PIN) == LOW) {
      Keyboard.press('n');
      delay(40);
      Keyboard.release('n');
      }
    // Check if 'a' button is presse 
    // Red   
    if (digitalRead(buttonAPin) == LOW) {
      Keyboard.press('p');
      delay(40);
      Keyboard.release('p');
      }

    // Check if 'd' button is pressed
    //Yellow
    if (digitalRead(buttonDPin) == LOW) {
      Keyboard.press('j');
      delay(40);
      Keyboard.release('j');
      }

    // Check if 'w' button is pressed
    //Green
    if (digitalRead(buttonWPin) == LOW) {
      Keyboard.press('w');
      delay(40);
      Keyboard.release('w');
      }    
}
static void NewPartControler(void)
{
    int xVal = analogRead(joystickX);
    int yVal = analogRead(joystickY);
    int swVal = digitalRead(joystickSW);
    // Map X axis to arrow keys
    PushX(xVal);
    // Map Y axis to arrow keys
    PushY(yVal);
    // Map switch to spacebar
    PushSpace(swVal);
    // Check if 's' button is pressed
    if (digitalRead(buttonSPin) == LOW) {
      Keyboard.press('s');
      delay(100);
      Keyboard.release('s');
      }
    delay(400); 
}
static void PartControler(void)
{
    int xVal = analogRead(joystickX);
    int yVal = analogRead(joystickY);
    int swVal = digitalRead(joystickSW);
    // Map X axis to arrow keys
    PushX(xVal);
    // Map Y axis to arrow keys
    PushY(yVal);
    // Map switch to spacebar
    PushSpace(swVal);
    // Check if 's' button is pressed
    if (digitalRead(buttonSPin) == LOW) {
      Keyboard.press('s');
      delay(100);
      Keyboard.release('s');
      }
    delay(100); 
}

void DisplayMessageScreen(int x, int y, const String& message, int d_time) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(x, y);  // Set cursor position to x, y
  lcd.print(message);  // Print the message
  delay(d_time);  // Delay for 2 seconds
  lcd.clear();  // Clear the LCD screen
}
void DisplayIntMessageScreen(int x, int y, int num,const String& message, int d_time) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(x,0);  // Set cursor position to x, y
  lcd.print(message);  // Print the message
  lcd.setCursor(x,1);  // Set cursor position to x, y
  lcd.print(num);  // Print the message
  delay(d_time);  // Delay for 2 seconds
  lcd.clear();  // Clear the LCD screen
}
void DisplayCalibProcess(int x, int y, int num,const String& message1,const String& message2 ,int d_time) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(5,0);  // Set cursor position to x, y
  lcd.print(message1);  // Print the message
  lcd.setCursor(0,1);  // Set cursor position to x, y
  lcd.print(message2);  // Print the message
  lcd.setCursor(13,1);  // Set cursor position to x, y
  lcd.print(num);  // Print the message
  delay(d_time);  // Delay for 2 seconds
  lcd.clear();  // Clear the LCD screen
}
void DisplayFloatMessageScreen(int x, int y, float num,const String& message, int d_time) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(x,0);  // Set cursor position to x, y
  lcd.print(message);  // Print the message
  lcd.setCursor(x,1);  // Set cursor position to x, y
  lcd.print(num);  // Print the message
  delay(d_time);  // Delay for 2 seconds
  lcd.clear();  // Clear the LCD screen
}
void DisplayTwoMessageScreen(int x,const String& message1,const String& message2) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(x,0);  // Set cursor position to x, y
  lcd.print(message1);  // Print the message
  lcd.setCursor(x,1);  // Set cursor position to x, y
  lcd.print(message2);  // Print the message
  delay(100);  // Delay for 2 seconds
}

void DisplayStage(int x,const String& message1,float low_num ,float high_num) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(x,0);  // Set cursor position to x, y
  lcd.print(message1);  // Print the message
  lcd.setCursor(x,1);  // Set cursor position to x, y
  lcd.print(low_num);  // Print the message
  lcd.setCursor(8,1);  // Set cursor position to x, y
  lcd.print(high_num);  // Print the message
  delay(200);  // Delay for 2 seconds
}
void DisplayStage3Num(int x,const String& message1,float vol,float low_num ,float high_num) 
{
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(x,0);  // Set cursor position to x, y
  lcd.print(message1);  // Print the message
  lcd.setCursor(0,1);  // Set cursor position to x, y
  lcd.print(low_num);  // Print the message
  lcd.setCursor(5,1);  // Set cursor position to x, y
  lcd.print('<');  // Print the message
  lcd.setCursor(6,1);  // Set cursor position to x, y
  lcd.print(vol);  // Print the message
  lcd.setCursor(10,1);  // Set cursor position to x, y
  lcd.print('<');  // Print the message
  lcd.setCursor(12,1);  // Set cursor position to x, y
  lcd.print(high_num);  // Print the message
  delay(500);  // Delay for 2 seconds
}
void displayMessage2Lines(const String& message1, const String& message2) {
  // Clear the LCD screen
  lcd.clear();
  
  // Print the message on the LCD
  lcd.setCursor(0, 0);
  lcd.print(message1);
  
  lcd.setCursor(0, 1);
  lcd.print(message2);
}

void Game_Enable(void)
{
  unsigned long GameTimeInSeconds = 0;
  unsigned long startGameTime;
  unsigned long GameElapsedTime;
  startGameTime = millis();
  while (GameTimeInSeconds <= GAME_TIME_TH)
  {
    GameElapsedTime = millis() - startGameTime;
    GameTimeInSeconds = GameElapsedTime / 1000;
    FullControler();
    DisplayCalibProcess(2,0,(GAME_TIME_TH - GameTimeInSeconds),"Enable","Time remain:",200);
  }  
}

///////////////////////////////Calibration///////////////////////////////
//////////////Timing and start clibration logic///////////////
int CalibFlag = 0;                                        
int start_treatment = 0;
int OmerInd = 1;
float startTime = millis();
float elapsedTime;
float TimeInSeconds;
float CalibelapsedTime;
float CalibTimeInSeconds;
//////////////calc volume vars///////////////

int SumLow = 0;
int SumHigh = 0;
int CountLow = 0;
int CountHigh = 0;
int MaxLungsAvarage;
int MinLungsAvarage;


///////////////////////////////Lunge Volume Calc///////////////////////////////
int LungVolume = 0;
int sensorValue;
int sensorValueOmer;
float pressure; // Variable to store pressure value
float AirFlowRate;
float filteredPressure = 0; // Variable to store filtered pressure value
float Patient_Lunge_Volume = 0;
float normalizedValue = 0;
int MAXVOLUME = 15;
float normalizedValue_Game = 0;

///////////////////////////////Active/InActive///////////////////////////////

float Zero_stage = 0;
float First_stage = 0;
float Second_stage = 0;
float Third_stage = 0;
float Delta_stage = 0;
int stage_counter = 0;
int Controler_Enable = 0;
int bcb=1;
int ButtomB;
int GameMode = 0;
int STAGE_TOTAL_LIMIT = 12;
int STAGE_REP_LIMIT = 4;
int END_TREATMENT = 4;

int GAME_LUNGE_TIME = 4;
float GameelapsedTime;
float startGameTime;
float GameTimeInSeconds;


void setup()
{
    delay(100);
    // put your setup code here, to run once:
    Serial.begin(9600); // Start serial communication at 9600 baud rate
    Keyboard.begin();
    lcd.begin(16, 2);
    lcd.print("Start Treatment");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(OPTIONS_PIN, INPUT_PULLUP);
    pinMode(buttonSPin, INPUT_PULLUP);
    pinMode(buttonAPin, INPUT_PULLUP);
    pinMode(buttonDPin, INPUT_PULLUP);
    pinMode(buttonWPin, INPUT_PULLUP);
    pinMode(joystickSW, INPUT_PULLUP);
    pinMode(joystickY, INPUT_PULLUP);
    pinMode(joystickX, INPUT_PULLUP);


}

void loop()
{
    int stage_counter = 0;
    int stage1_counter = 0;
    int stage2_counter = 0;
    int stage3_counter = 0;
    int current_stage = 0;
    int stage_global_counter = 0;
    int stage_state = -1;
    int Enable = 1;
    int ButtomA = 1;

    while (start_treatment == 0)
    {
      DisplayTwoMessageScreen(0,"Please Press Calib","buttom to start");
      ButtomA = digitalRead(BUTTON_PIN);
      if(ButtomA == 0)
      {
        lcd.clear();  // Clear the LCD screen
        start_treatment = 1;
      }
      FullControler();
      //Joystick();
    
    }

    //Condition for the start-reset logic
    if (start_treatment == 1 && CalibFlag==0 ){
        CalibFlag = 1;
        Serial.print("Start Calibration");
        DisplayMessageScreen(2,0,"Start Calib",2000);
    }
    unsigned long CalibTimeInSeconds = 0;
    unsigned long startCalibTime;
    //unsigned long CalibelapsedTime;
   // unsigned long GameTimeInSeconds = 0;
    //unsigned long startGameTime;
    //unsigned long GameElapsedTime; 
    //Serial.print(ButtomA);
    //Serial.print(BUTTON_PIN);

    if (CalibFlag == 1)
    {
        //DisplayMessageScreen(2,0,"Var Init",400);
        startCalibTime = millis();
       // MinLungsAvarage = 0;
        LungVolume = 0;
        SumHigh = 0;
        CountHigh =0;
        SumLow = 0;
        CountLow = 0;
    }
    while ((CalibFlag == 1) && (CalibTimeInSeconds <= CALIB_TIME_TH))
    {
        int ButtomA = digitalRead(BUTTON_PIN);
        //Condition for the start-reset logic
         if (ButtomA == 0 && CalibFlag==0 )
         {
          CalibFlag = 1;
          Serial.print("Start Calibration");
          DisplayMessageScreen(2,0,"Start Calib", 2000);

        }
        //delay(1000);
        // Read the analog value from the sensor
        sensorValue = analogRead(pressurePin); 
        pressure = 3*(sensorValue * 4.0 / 1023.0) - BIAS;
        filteredPressure = alpha * pressure + (1 - alpha) * pressure; // Apply the Gaussian filter to the pressure reading
        //DisplayIntMessageScreen(2,0,filteredPressure,"FP:",1000);
        Serial.print("Pressure:\n"); // Print the pressure value to the serial monitor
        Serial.print(filteredPressure);
        Serial.println(" kPa\n");
        //AIR_FLOW_RATE FUNTION!
        float C = (1 / (D1 * D1) - (1 / (D2 * D2)));
        if (pressure >0)//check if pressure or filtered pressure
        {
            AirFlowRate = sqrt((2 * (-1*pressure)) / (C * density));
        }
        
        if (pressure <=0)//check if pressure or filtered pressure
        {
            AirFlowRate = (-1)*sqrt((2 * pressure) / (C * density));
        }   
        //AirFlowRate = *sqrt((2 * pressure) / (C * density));
        //PRESSURE FUNC END
        Serial.print("AirFlowRate: \n"); // Print the pressure value to the serial monitor
        Serial.print(AirFlowRate);
        Serial.println(" Vol/sec\n");
        elapsedTime = millis() - startTime;
        TimeInSeconds = elapsedTime / 1000;
        if (abs(AirFlowRate)>CALIB_INTEGRAL_THRESHOLD){
            LungVolume = LungVolume + TimeInSeconds * AirFlowRate;
        }
        Serial.print("Vol: \n"); // Print the pressure value to the serial monitor
        Serial.print(LungVolume);
        Serial.println(" litter\n");
        if (LungVolume<=MinLungsThreshold){
          SumLow=SumLow+LungVolume;
          CountLow++;
        }
        if (LungVolume>=MaxLungsThreshold){
          SumHigh=SumHigh+LungVolume;
          CountHigh++;
        }
        CalibelapsedTime = millis() - startCalibTime;
        CalibTimeInSeconds = CalibelapsedTime / 1000;
        if (CalibTimeInSeconds< CALIB_TIME_TH/2)
        {
          DisplayCalibProcess(2,0,(CALIB_TIME_TH/2 - CalibTimeInSeconds),"INHALE","Time remain:",200);
          //DisplayCalibProcess(2,0,(AirFlowRate*1000),"INHALE","Time remain:",200);

        }
        if (CalibTimeInSeconds >= CALIB_TIME_TH/2)
        {
          DisplayCalibProcess(2,0,(CALIB_TIME_TH - CalibTimeInSeconds),"EXHALE","Time remain:",200);
          //DisplayCalibProcess(2,0,(AirFlowRate*1000),"INHALE","Time remain:",200);
        }
        //DisplayIntMessageScreen(2,0,CalibTimeInSeconds,"Time passed:",200);
        //DisplayIntMessageScreen(2,0,(LungVolume),"Vol:",200);
        // Serial.print("CalibTime : ");
        //Serial.print(CalibTimeInSeconds);
        //lcd.setCursor(2,1);
        //lcd.print(CalibTimeInSeconds);
        //lcd.clear();
        //Serial.print("\n");
        //DisplayIntMessageScreen(2,0,CalibTimeInSeconds,"Cal time:");
        //delay(100);
        if (CalibTimeInSeconds >= 10)
        {
          //DisplayIntMessageScreen(2,0,CalibTimeInSeconds,"Calib Time:",2000);

          CalibFlag = 0;
          MaxLungsAvarage = SumHigh/CountHigh;
          MinLungsAvarage = SumLow/CountLow;

          Patient_Lunge_Volume = (MaxLungsAvarage - MinLungsAvarage);
          normalizedValue = (Patient_Lunge_Volume / MAXVOLUME) * 9.0;
          DisplayFloatMessageScreen(2,0,normalizedValue,"P L V",2000);
          //Normalize to 6 !!!!!!
          //DisplayFloatMessageScreen(2,0,Patient_Lunge_Volume,"PLV",2000);
          //DisplayFloatMessageScreen(2,0,MinLungsAvarage,"MinLungsAvarage",2000);
          //Zero_stage = MinLungsAvarage;
          //First_stage = MinLungsAvarage + Patient_Lunge_Volume/3;
          //Second_stage = MinLungsAvarage + 2*Patient_Lunge_Volume/3;
          //Delta_stage = (BETA*Patient_Lunge_Volume)/3;
          //Third_stage = MinLungsAvarage + Patient_Lunge_Volume;
          
          Zero_stage = 0;
          First_stage = normalizedValue/3;
          Second_stage = 2*normalizedValue/3;
          Delta_stage = (BETA*normalizedValue)/3;
          Third_stage =  normalizedValue;

          normalizedValue = 0;
          LungVolume = 0;
          SumHigh = 0;
          CountHigh =0;
          SumLow = 0;
          CountLow = 0;
          DisplayMessageScreen(2,0,"Calib Ended", 2000);
          GameMode = 1;
          DisplayFloatMessageScreen(2,0,Zero_stage,"Zero stage",2000);
          DisplayFloatMessageScreen(2,0,First_stage,"First Stage",2000);
          DisplayFloatMessageScreen(2,0,Second_stage,"Second stage",2000);
          DisplayFloatMessageScreen(2,0,Third_stage,"Third stage",2000);


        }
    }
  LungVolume = -1;
  while (CalibFlag == 0 && GameMode == 1) // Game
    {
        OmerInd = OmerInd +1;
        //LungVolume = -2;
        //AirFlowRate = 0;
        //pressure = 0;
        GameTimeInSeconds = 0;
        int ButtomA = digitalRead(BUTTON_PIN);
        //Condition for the start-reset logic
        if (ButtomA == 0 )
        {
          CalibFlag = 1;
          DisplayMessageScreen(2,0,"Start Calib",2000);
          delay(1500);
          //break;
        }

        //delay(1000);
        // Read the analog value from the sensor
        //startGameTime = millis();
        while(GameTimeInSeconds <= GAME_LUNGE_TIME*80)
        {
          PartControler();
          //DisplayMessageScreen(2,0,"Inhale/Exhale",200);
          //DisplayCalibProcess(2,0,(GameTimeInSeconds),"COSEMEK","Time remain:",200);
          sensorValue = analogRead(pressurePin); 
          sensorValueOmer = analogRead(pressurePin); 
          pressure = 3*(sensorValue * 4.0 / 1023.0) - BIAS;
          filteredPressure = alpha * pressure + (1 - alpha) * pressure; // Apply the Gaussian filter to the pressure reading
          Serial.print("Pressure:\n"); // Print the pressure value to the serial monitor
          Serial.print(sensorValueOmer);
          Serial.println(" kPa\n");
          //AIR_FLOW_RATE FUNTION!
          float C = (1 / (D1 * D1) - (1 / (D2 * D2)));
          if (pressure >0)//check if pressure or filtered pressure
          {
            AirFlowRate = sqrt((2 * (-1*pressure)) / (C * density));
          }
        
          if (pressure <=0)//check if pressure or filtered pressure
          {
            AirFlowRate = (-1)*sqrt((2 * pressure) / (C * density));
          }  
          //AirFlowRate = abs(AirFlowRate); 
          //AirFlowRate = *sqrt((2 * pressure) / (C * density));
          //PRESSURE FUNC END
          Serial.print("AirFlowRate: \n"); // Print the pressure value to the serial monitor
          Serial.print(sensorValueOmer);
          //DisplayIntMessageScreen(3,1,100*AirFlowRate,"FlowRate:");
          Serial.println(" Vol/sec\n");
          GameelapsedTime = millis() - startGameTime;
          GameTimeInSeconds = GameelapsedTime / 1000;
          if (abs(AirFlowRate)>PLAIN_INTEGRAL_THRESHOLD)
          {
            LungVolume = LungVolume + TimeInSeconds * AirFlowRate;
          }
          //DisplayFloatMessageScreen(2,0,LungVolume,"LungVolume",200);
          //Omer ADDITION
          if (sensorValueOmer <= 560 )
          {
          PartControler();
          DisplayTwoMessageScreen(0,"Joystick enable","Buttons disabled");               
          }
          else
          {
           Game_Enable();
          }
        }
  }
}