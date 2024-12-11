//************************************************************************************************
// ESPEncoderNeopixel.ino
// Uses an ESP CPU and an Encode with switch to run through LED programs.
//
// Button = SCK
// Yellow = MOSI
// Orange = MISO 
// Led Strip IO33
//
//
// RCI 10-30-2024
//************************************************************************************************
#include <AiEsp32RotaryEncoder.h>
#include <Adafruit_NeoPixel.h>
#include "WS2811_Patterns.h"

#define pinPullups 0    // 0 = Pullup, 1 = PullDown

#if defined(ESP32)
#define ROTARY_ENCODER_A_PIN      25  // MISO
#define ROTARY_ENCODER_B_PIN      27  // MOSI
#define ROTARY_ENCODER_BUTTON_PIN 26  // SCK
#endif

#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN      13  // MISO
#define ROTARY_ENCODER_B_PIN      12  // MOSI
#define ROTARY_ENCODER_BUTTON_PIN 14  // SCK
#endif

#define ROTARY_ENCODER_STEPS 4


unsigned long time_now = 0;
int brightness = 127;
int progNum = 0;
//extern bool buttonPressedFlag = 0;

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, 
                                     ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS, pinPullups);
//******************************************************************************************
void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}
//************************************************************************************************
// Delay (loops) Milliseconds
void delayms(int loops) 
{
    time_now = millis();
    
    while(millis() < time_now + loops)
      {
      brightness = rotaryEncoder.readEncoder();
      strip.setBrightness(brightness);
      //Serial.println(rotaryEncoder.readEncoder());

      if (rotaryEncoder.isEncoderButtonClicked())
        {
        //Serial.println("button pressed");
        buttonPressedFlag = 1;
        progNum++;
        //break;
        }
      delay(1);   
      } 
           
}
//******************************************************************************************
void setup()
{
    Serial.begin(115200);
    delay(3000);
    pinMode(PIN, OUTPUT);
    
    rotaryEncoder.begin();
    //rotaryEncoder.areEncoderPinsPulldownforEsp32 = false;
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 255, false); 
    rotaryEncoder.setAcceleration(250);
    brightness = rotaryEncoder.readEncoder();
    progNum = 0;
   
}
//******************************************************************************************
void loop()
{

    
    if(progNum > 11) progNum = 0;

    switch(progNum)
       {
       case 0:
       SolidColor(wht, 1);
       delayms(1);
       break;
       //****************************
       case 1:
       SolidColor(rde, 1);
       delayms(1);
       break;
       //****************************
       case 2:
       SolidColor(grn, 1);
       delayms(1);
       break;
       //****************************
       case 3:
       SolidColor(blu, 1);
       delayms(1);
       break;
       //****************************
       case 4:
       SolidColor(yellow, 1);
       delayms(1);
       break;
       //****************************
       case 5:
       SolidColor(magenta, 1);
       delayms(1);
       break;
       //****************************
       case 6:
       SolidColor(cyan, 1);
       delayms(1);
       break;
       //****************************
       case 7:

       //theaterChaseRainbowFor(3, 1, 150); //  fixed rainbow color with chased position
       theaterChaseRainbow(3, 1, 150); // skipt, cycles, wait

       break;
       //****************************
       case 8:

       rainbowStrip(25);   // delay

       break;
       //****************************
       case 9:

       rainbowCycleFor(1, 25); 

       break;
       //****************************
       case 10:
       MultiCometRev(wht, 15, 1, 100);  // skipt, cycles, wait 
       delayms(1);
       break;
       //****************************
       case 11:

       RandomColorStrip(2, 1, 350); 

       break;                     
       //****************************
       default:

       break;       
       }



      


}
//******************************************************************************************
