#include <Keyboard.h>
#define BUTTON_PIN 4
#define LEFT_PIN 5
#define RIGHT_PIN 6
#define UP_PIN 7
#define DOWN_PIN 8
#define FAKE_CALIB_PIN 8

void setup()
{
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
    byte CalibState = digitalRead(FAKE_CALIB_PIN);
    byte Rstate = digitalRead(RIGHT_PIN);
    byte Lstate = digitalRead(LEFT_PIN);
    byte Ustate = digitalRead(UP_PIN);
    byte Dstate = digitalRead(DOWN_PIN);
    byte Estate = digitalRead(BUTTON_PIN);



    if (CalibState == LOW)
    {
        if (Rstate == LOW)
        {
            Keyboard.press(KEY_RIGHT_ARROW); // Press RIGHT
        }
        if (Lstate == LOW)
        {
            Keyboard.press(KEY_LEFT_ARROW); // Press LEFT
        }
        if (Ustate == LOW)
        {
            Keyboard.press(KEY_DOWN_ARROW); // Press DIWN
        }
        if (Dstate == LOW)
        {
            Keyboard.press(KEY_UP_ARROW); // Press UP
        }
    }
    delay(100);
}