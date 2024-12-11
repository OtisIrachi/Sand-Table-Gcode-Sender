
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

unsigned char leds_per_letter[13] = {0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

unsigned int letter_led_position[12][2] = {
                                       {0, 0},
                                       {0, 7},    // F
                                       {8, 11},   // I
                                       {12, 19},  // V
                                       {20, 28},  // E
                                       {29, 43},  // M
                                       {44, 52},  // A
                                       {53, 64},  // N
                                       {65, 71},  // T
                                       {72, 81},  // R
                                       {82, 85},  // I
                                       {86, 98},  // O
                                       };
                                       
#define F_  	1
#define I_  	2
#define V_  	3
#define E_  	4
#define M_  	5
#define A_  	6
#define N_  	7
#define T_  	8
#define R_  	9
#define I2_ 	10
#define O_  	11

#define blackm       strip.Color(0, 0, 0)        // black
#define redm         strip.Color(255, 0, 0)      // red
#define bluem        strip.Color(0, 0, 255)      // blue
#define greenm       strip.Color(0, 255, 0)      // green                           
#define whitem       strip.Color(255, 255, 255)  // white
#define dimyellowm   strip.Color(127, 40, 0)     // yellow
#define yellowm      strip.Color(255, 255, 0)    // yellow
#define magentam     strip.Color(255, 0, 255)    // magenta
#define cyanm        strip.Color(0, 255, 255)    // cyan                      
#define orangem      strip.Color(255, 75, 0)     // orange 
#define goldm        strip.Color(255, 150, 0)    // gold
#define limem        strip.Color(30, 255, 100)   // lime
#define purplem      strip.Color(100, 0, 150)  
#define randColorm   strip.Color(random(255), random(255), random(255)) 
#define randomColorPrimary pixelcolors[random(22) + 1][0], pixelcolors[random(22) + 1][1], pixelcolors[random(22) + 1][2]  
                                                                                                 
#define rde       255, 0, 0
#define	grn	      0, 255, 0
#define	blu	      0, 0, 255
#define	blk	      0 ,0, 0
#define	grey	    1, 1, 1
#define dimwht    1, 1, 1
#define midwht    127, 127, 127
#define wht       255, 255, 255
#define	coolwht	  255, 255, 255
#define	yellow	  255, 138, 0
#define	cyan	    0, 255, 255
#define	magenta	  255, 0, 24
#define	gold	    255, 90, 0
#define	orange	  220, 40, 0
#define	olive	    100, 220, 47
#define	turq	    35, 255, 30
#define	lime	    15, 70, 10
#define	skyblue	  65, 176, 255
#define	purple	  70, 0, 120
#define	pink	    59, 6, 10
#define	crimson	  220, 3, 3
#define darkred   30, 0, 0
#define navy      0, 0, 128
#define teal      0, 128, 128
#define midnightblue  0, 0, 3
#define mintgrn   18, 71, 12
#define copper    255, 48, 6
#define warmwht   255, 91, 11
#define indigo    5, 0, 15

#define num_of_pixels 71

#if defined(ESP8266)
#define PIN 4
#endif

#if defined(ESP32)
#define PIN 21
#endif

bool buttonPressedFlag = 0;
int AbortProgram();
void ReadEnc(); 
void delayms(int loops);
void FadePixel(int pos, char rrednext, char ggreennext, char bbluenext,
                char rredlast, char ggreenlast, char bbluelast,
                int fadrate); 
String pixColors[23] = {"dimwht", "red", "green", "blue", "warmwht", "darkred", "midwht", "coolwht", "yellow", "cyan", "magenta", 
                         "gold", "orange", "olive", "turq", "purple", "pink", "lime", "skyblue", "teal", "mintgreen", "copper", "indigo"}; 

unsigned char pixelcolors[23][3] = {
                            {1, 1, 1},        // dimwht
                            {255, 255, 255},  // coolwht                           
                            {255, 0, 0},      // red
                            {0, 255, 0},      // green
                            {0, 0, 255},      // blue
                            {255, 91, 11},    // warmwht
                            {1, 0, 0},        // darkred
                            {50, 50, 50},     // midwht                          
                            {255, 138, 0},    // yellow
                            {0, 255, 255},    // cyan
                            {255, 0, 24},     // magenta
                            {255, 90, 0},     // gold
                            {220, 40, 0},     // orange 
                            {100, 220, 47},   // olive 
                            {35, 255, 70},    // turq
                            {70, 0, 120},     // purple 
                            {56, 6, 10},      // pink                            
                            {15, 70, 10},     // lime   
                            {65, 176, 255},   // skyblue   
                            {0, 128, 128},    // teal
                            {18, 71, 12},     // mintgrn
                            {255, 48, 6},     // copper 
                            {5, 0, 15},       // indigo                                                                                                         
                            };


signed int j, v;
unsigned char checkEnc;
unsigned long shiftPos[num_of_pixels];
int MasterFade;
int array_line;
int row, ledpos, array_row;
int SEGLEN;
// blk Strip = 150 LEDS
Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_of_pixels, PIN, NEO_GRB + NEO_KHZ800);
   

//*********************************************************************************
// The colours are a transition r - g - b - back to r.
long Wheel(byte WheelPos) 
{
  if(WheelPos < 85)  
      {
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      } 
  else if(WheelPos < 170) 
      {
      WheelPos -= 85;
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
      } 
  else 
      {
      WheelPos -= 170;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
}
//****************************************************************************
unsigned long RGBtoLong(unsigned long r, unsigned int g, unsigned char b)
{
   unsigned long temp, temp1;
   temp = 0;
   temp1 = 0;
   temp1 = r << 16;
   temp = temp + temp1;
   temp1 = 0;
   temp1 = g << 8;
   temp = temp + temp1;
   temp1 = 0;
   temp1 = b;
   temp = temp + temp1;
   return temp;  
}
//************************************************************************************************
void SectionColor(unsigned int startpos, unsigned int endpos, unsigned long c, int wait)
{
int pos;

    for(pos = startpos; pos < endpos; pos++)
       {
       ReadEnc(); 
       strip.setPixelColor(pos, c);            
       }
       strip.show();
       delayms(wait); 
}
//****************************************************************************
void SectionColor(unsigned int startpos, unsigned int endpos, unsigned char r, unsigned char g, unsigned char b, int wait)
{
int pos;

    for(pos = startpos; pos < endpos; pos++)
       {
       ReadEnc(); 
       strip.setPixelColor(pos, strip.Color(r, g, b));            
       }
       strip.show();
       delayms(wait); 
}
//****************************************************************************
void SolidColor(unsigned long c, int spaces)
{
int pos;

    for(pos = 0; pos < strip.numPixels(); pos = pos + spaces)
       {
       strip.setPixelColor(pos, c);            
       }
       strip.show(); 
}
//****************************************************************************
void SolidColor(unsigned char r, unsigned char g, unsigned char b, int spaces)
{
int pos;

    for(pos = 0; pos < strip.numPixels(); pos = pos + spaces)
       {
       strip.setPixelColor(pos, strip.Color(r, g, b));            
       }
       strip.show();

}
//****************************************************************************
void RandomColor(int skipt, int cycles, int wait)
{
  int pixelpos, j; 

  for (j = 0; j < cycles; j++) 
    {
    for(pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt)
        {
         
        strip.setPixelColor(pixelpos, pixelcolors[random(22)][3], pixelcolors[random(22)][3], pixelcolors[random(22)][3]);                  
        }
        strip.show();
        delayms(wait);
    }   
}
//******************************************************   
//RandomColors(0, 0, random(7) + 1, 50, 30);
void RandomColorStrip(int skipt, int repeats, int wait) 
{
  int x, y, z;
  int times, pixelpos;
  
  //Random pallet of colors for frame, then delay
  for (times = 0; times < repeats; times++)
    {
    for(x = 0; x < num_of_pixels; x = x + skipt)
      {
      pixelpos = random(num_of_pixels);    
      strip.setPixelColor(pixelpos, pixelcolors[random(22)][3], pixelcolors[random(22)][3], pixelcolors[random(22)][3]);
      pixelpos = random(num_of_pixels);    
      strip.setPixelColor(pixelpos, 0, 0, 0);       
      }

    strip.show();
    delayms(wait);  
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }           
    }
   
}
//*********************************************************************************
// Fill the dots one after the other with a color
void colorDotFor(unsigned char r, unsigned char g, unsigned char b, unsigned int wait) 
{
int i;
  
  for(i = 0; i < strip.numPixels(); i++) 
      {
      strip.setPixelColor(i, r, g, b);  
      strip.setPixelColor(i - 1, 0x000000);
       
      strip.show();
      delayms(wait);
      }
}
//*********************************************************************************
// Fill the dots one after the other with a color
void colorDotRev(unsigned char r, unsigned char g, unsigned char b, unsigned int wait) 
{
int i; 
 
  for (i = strip.numPixels(); i > -1; i = i + -1) 
  //for(i = strip.numPixels(); i--;)
      {
      strip.setPixelColor(i, r, g, b );   
      strip.setPixelColor(i + 1, 0x000000);
       
      strip.show();
      delayms(wait);
      }
}
//*********************************************************************************
// Fill the dots one after the other with a color
void colorWipeFor(unsigned char r, unsigned char g, unsigned char b, int spacing, unsigned int wait) 
{
  int i;
  
  for(i = 0; i < strip.numPixels(); i = i + spacing) 
      {
      strip.setPixelColor(i, r, g, b);   
      //strip.setPixelColor(i, c);
      ReadEnc();
      strip.show();
      delayms(wait);
      }
}
//*********************************************************************************
// Fill the dots one after the other with a color
void colorWipeRev(unsigned char r, unsigned char g, unsigned char b, int spacing, unsigned int wait) 
{
  int i;
 
  for(i = strip.numPixels(); i > -1; i = i - spacing)
      {
      strip.setPixelColor(i, r, g, b);   
      //strip.setPixelColor(i, c);
      ReadEnc();
      strip.show();
      delayms(wait);
      }
}

//*********************************************************************************
void rainbowStrip(unsigned int wait) 
{
  int pixelpos, repeat;

  for(repeat = 0; repeat < 256; repeat++) 
      {
      for(pixelpos = 0; pixelpos < strip.numPixels(); pixelpos++) 
          {
          strip.setPixelColor(pixelpos, Wheel((pixelpos + repeat) & 255));
           
          }
      strip.show();
      delayms(wait);
      if(buttonPressedFlag == 1) 
        {
        buttonPressedFlag = 0;
        return;
        }
      }
    
}
//*********************************************************************************
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleFor(unsigned int cycles, unsigned int wait) 
{
int pixelpos, loops;
  
  for(loops = 0; loops < 256 * cycles; loops++)     // 5 cycles of all colors on wheel
      { 
      for(pixelpos = 0; pixelpos < strip.numPixels(); pixelpos++) 
          {
          strip.setPixelColor(pixelpos, Wheel(((pixelpos * 256 / strip.numPixels()) + loops) & 255));           
          }
          strip.show();
          delayms(wait);
          if(buttonPressedFlag == 1) 
            {
            buttonPressedFlag = 0;
            return;
            }          
      }
}
//*********************************************************************************
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleRev(unsigned int cycles, unsigned int wait) 
{
int pixelpos, loops;
  
  for(loops = 256 * cycles; loops--;)    // 5 cycles of all colors on wheel
      { 
      for(pixelpos = 0; pixelpos < strip.numPixels(); pixelpos++) 
          {
          strip.setPixelColor(pixelpos, Wheel(((pixelpos * 256 / strip.numPixels()) + loops) & 255));           
          }
          strip.show();
          delayms(wait);
          if(buttonPressedFlag == 1) 
            {
            buttonPressedFlag = 0;
            return;
            }           
      }
}
//*****************************************************************************************
void chaseBlocks(long c1, long c2, long c3, int cycles, int colorsize, int wait) 
{
   int st1 = 0;
   int st2 = colorsize;
   int st3 = st2 + colorsize;
   int st4 = st3 + colorsize;
   int start1, start2, start3, start4;
   int pixelpos, pos, loops, spacing, a, b, c, d, skipt;

    start1 = st1 + colorsize;    
    start2 = st2 + colorsize;    
    start3 = st3 + colorsize;
    start4 = st4 + colorsize;  
      
    skipt = strip.numPixels() * 2;

  for (loops = 0; loops < cycles; loops++)      
    {
    for (spacing = 0; spacing < skipt; spacing++) 
      {         
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos++) // pixelpos = pixelpos + skipt
        {
        for(a = st1; a < start1; a++)   //red
          {
          strip.setPixelColor(pixelpos + spacing - a, c1);                
          }        
        for(b = st2; b < start2; b++)   //green
          {  
          strip.setPixelColor(pixelpos + spacing - b, c2);           
          }       
        for(c = st3; c < start3; c++)   //blue
          {
          strip.setPixelColor(pixelpos + spacing - c, c3);                          
          }      
          strip.setPixelColor(pixelpos + spacing - start3, 0x000000);  
        
        strip.show();
        delayms(wait);

        for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos++) 
          {
          strip.setPixelColor(pixelpos + spacing, 0);             // turn every third pixel off
          }
          
        }// End pixelpos
      }// End spacing       
   }// End loops    
}
//****************************************************************************
void Chase_3Color_For(long c1, long c2, long c3, int skipt, int cycles, uint8_t wait)
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)      
    {  
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing + 2, c3);        // turn every third pixel on
        strip.setPixelColor(pixelpos + spacing + 1, c2);        // turn every third pixel on
        strip.setPixelColor(pixelpos + spacing, c1);            // turn every third pixel on         
        }
        
      strip.show();
      delayms(wait);

      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);             // turn every third pixel off
        }
        
      }// End spacing
      
    }// End loops
    
}// End Chase_3Color_For
//****************************************************************************
void Chase_3Color_Rev(long c1, long c2, long c3, signed int skipt, int cycles, uint8_t wait)
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)     
    {  
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = strip.numPixels(); pixelpos > -1; pixelpos = pixelpos + -skipt) 
        {
        strip.setPixelColor(pixelpos - spacing - 2, c3);        // turn every third pixel on
        strip.setPixelColor(pixelpos - spacing - 1, c2);        // turn every third pixel on  
        strip.setPixelColor(pixelpos - spacing, c1);            // turn every third pixel on         
        }
        
      strip.show();
      delayms(wait);
      
      for (pixelpos = strip.numPixels(); pixelpos > -1; pixelpos = pixelpos + -skipt) 
        {
        strip.setPixelColor(pixelpos - spacing, 0);             // turn every third pixel off
        }
        
      }// End spacing
      
    }// End loops
    
}// End Chase_3Color_Rev
//****************************************************************************
// Theatre-style crawling lights.
// Each frame will be a different rainbow color
void theaterChaseRainbow(int skipt, int cycles, int wait) 
{
  int pixelpos, spacing, loops;

  for (loops = 0; loops < 256 * cycles; loops++)      
    {             
    for (spacing = 0; spacing < skipt; spacing++) 
      {                 
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, Wheel(((pixelpos * 256 / strip.numPixels()) + loops) & 255));       
        }
        
        strip.show();      
        delayms(wait);
        if(buttonPressedFlag == 1) 
           {
           buttonPressedFlag = 0;
           return;
           }
       
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);        // turn every third pixel off
        }
        
      }// End spacing/skipt
    }// End Loops
}// End  theaterChaseRainbow
//****************************************************************************
//Theatre-style crawling lights.
//void theaterChaseFor(uint32_t c, int spacing, int cycles, uint8_t wait) 
void theaterChaseFor(char r, char g, char b, int skipt, int cycles, int wait) 
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)      // loop 100 times
    { 
    // q allows strip pos to increment by one 
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, r, g, b );              
        }
      strip.show();      
      delayms(wait);
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }       
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);        
        }
      }
    }
}
//****************************************************************************
//Theatre-style crawling lights.
//void theaterChaseRev(uint32_t c, int spacing, int cycles, uint8_t wait) 
void theaterChaseRev(char r, char g, char b, int skipt, int cycles, int wait)
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)      // loop 100 times
    {
       
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = strip.numPixels(); pixelpos > -1; pixelpos = pixelpos + -skipt) 
        {
        strip.setPixelColor(pixelpos - spacing, r, g, b);     
        }
      strip.show();       
      delayms(wait); 
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }
      for (pixelpos = strip.numPixels(); pixelpos > -1; pixelpos = pixelpos + -skipt) 
        {
        strip.setPixelColor(pixelpos - spacing, 0);        // turn every third pixel off       
        }
      }
    }
}
//****************************************************************************
//Theatre-style crawling lights with fixed rainbow effect
void theaterChaseRainbowFor(int skipt, int cycles, int wait) 
{
int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++) 
    {
    // cycle all 256 colors in the wheel
    for (spacing = 0; spacing < 3; spacing++) 
      {
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, Wheel( (pixelpos + loops) % 255));    // turn every third pixel on         
        }
        
      strip.show();
      delayms(wait);
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);                        // turn every third pixel off
        }
      
      }// End q
    
    }// End loops
}
//****************************************************************************
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbowRev(int skipt, int cycles, int wait) 
{
int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++) 
    {    
    // cycle all 256 colors in the wheel
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = strip.numPixels(); pixelpos > -1; pixelpos = pixelpos + -skipt)    
        {
        strip.setPixelColor(pixelpos - spacing, Wheel( (pixelpos + loops) % 255));    // turn every third pixel on
         
        }
      strip.show();
      delayms(wait);
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }      
      for (pixelpos = strip.numPixels(); pixelpos > -1; pixelpos = pixelpos + -skipt) 
        {
        strip.setPixelColor(pixelpos - spacing, 0);                        // turn every third pixel off
        }
      }
    }
}
//*********************************************************************************
void CometFor(unsigned char rred, unsigned char ggreen, unsigned char bblue, unsigned int wait) 
{
int pixelpos;
  
  for(pixelpos = 0; pixelpos < strip.numPixels() + 9; pixelpos++) 
      {        
      strip.setPixelColor(pixelpos, rred, ggreen, bblue); 
      strip.setPixelColor(pixelpos - 1, rred / 2, ggreen / 2, bblue / 2);
      strip.setPixelColor(pixelpos - 2, rred / 4, ggreen / 4, bblue / 4);
      strip.setPixelColor(pixelpos - 3, rred / 8, ggreen / 8, bblue / 8);
      strip.setPixelColor(pixelpos - 4, rred / 16, ggreen / 16, bblue / 16);
      strip.setPixelColor(pixelpos - 5, rred / 32, ggreen / 32, bblue / 32);
      strip.setPixelColor(pixelpos - 6, rred / 64, ggreen / 64, bblue / 64);
      strip.setPixelColor(pixelpos - 7, rred / 128, ggreen / 128, bblue / 128);
      strip.setPixelColor(pixelpos - 8, 0x000000);      
      strip.show();
      delayms(wait);
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }      
      }

}
//*********************************************************************************
void CometRev(unsigned char rred, unsigned char ggreen, unsigned char bblue, unsigned int wait) 
{
int pixelpos;
  
  for(pixelpos = strip.numPixels() + 9; pixelpos--;)
      {
           
      strip.setPixelColor(pixelpos - 9 , rred, ggreen, bblue); 
      strip.setPixelColor(pixelpos - 8, rred / 2, ggreen / 2, bblue / 2);
      strip.setPixelColor(pixelpos - 7, rred / 4, ggreen / 4, bblue / 4);
      strip.setPixelColor(pixelpos - 6 , rred / 8, ggreen / 8, bblue / 8);
      strip.setPixelColor(pixelpos - 5, rred / 16, ggreen / 16, bblue / 16);
      strip.setPixelColor(pixelpos - 4, rred / 32, ggreen / 32, bblue / 32);
      strip.setPixelColor(pixelpos - 3, rred / 64, ggreen / 64, bblue / 64);
      strip.setPixelColor(pixelpos - 2, rred / 128, ggreen / 128, bblue / 128);
      strip.setPixelColor(pixelpos - 1, 0x000000);
      strip.show();
      delayms(wait);
      if(buttonPressedFlag == 1) 
         {
         buttonPressedFlag = 0;
         return;
         }      
      }
      strip.setPixelColor(0, 0x000000);
      strip.show();      
}
//*********************************************************************************
void MultiColorScroll(long c1, long c2, long c3, long c4, long c5, 
                      int skipt, int spaces, int cycles, int wait)
{
  int pixelpos, spacing, loops, x, y, z;
  int patternrepeat;
 
  //spaces = 2;
  skipt = skipt + spaces;        // places pixel position for patterns every skipt
  patternrepeat = skipt;         // how far scroll moves pattern within the strip


  if(c1 == 0) patternrepeat = patternrepeat - 1;
  if(c2 == 0) patternrepeat = patternrepeat - 1;
  if(c3 == 0) patternrepeat = patternrepeat - 1;
  if(c4 == 0) patternrepeat = patternrepeat - 1;
  if(c5 == 0) patternrepeat = patternrepeat - 1;
 
  for (loops = 0; loops < cycles; loops++)      
    {  
    for (spacing = 0; spacing < patternrepeat; spacing++) // spacing makes scroll
      {
      //spacing = 0;
      for (pixelpos = 0; pixelpos < strip.numPixels() + skipt; pixelpos = pixelpos + skipt) 
        {
           // skipt places this series every skipt position
           if(c1 > 0) strip.setPixelColor(pixelpos + spacing - 0, c1);
                     
           if(c2 > 0) strip.setPixelColor(pixelpos + spacing - 1, c2);
                       
           if(c3 > 0) strip.setPixelColor(pixelpos + spacing - 2, c3);
           
           if(c4 > 0) strip.setPixelColor(pixelpos + spacing - 3, c4);
           
           if(c5 > 0) strip.setPixelColor(pixelpos + spacing - 4, c5); 
                                     
        }// End pattern
      
      strip.show();
      delayms(wait);
      strip.clear();    // just clears the buffer
      strip.show();

      }// End spacing
      
    }// End loops
    
}// End MultiColorScroll
//*********************************************************************************
void MultiCometFor(unsigned char rred, unsigned char ggreen, unsigned char bblue, int skipt, int cycles, unsigned int wait) 
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)      
    {  
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
         
        // "1 thru 9" changes how comet is pointed  
        // pixelpos + spacing changes scroll direction
        strip.setPixelColor(pixelpos + spacing - 1 , rred, ggreen, bblue);                    
        strip.setPixelColor(pixelpos + spacing - 2, rred / 2, ggreen / 2, bblue / 2);     
        strip.setPixelColor(pixelpos + spacing - 3, rred / 4, ggreen / 4, bblue / 4);
        strip.setPixelColor(pixelpos + spacing - 4 , rred / 8, ggreen / 8, bblue / 8);
        strip.setPixelColor(pixelpos + spacing - 5, rred / 16, ggreen / 16, bblue / 16);
        strip.setPixelColor(pixelpos + spacing - 6, rred / 32, ggreen / 32, bblue / 32);
        strip.setPixelColor(pixelpos + spacing - 7, rred / 64, ggreen / 64, bblue / 64);
        strip.setPixelColor(pixelpos + spacing - 8, rred / 128, ggreen / 128, bblue / 128);
        strip.setPixelColor(pixelpos + spacing - 9, 0x000000);  
        }
      strip.show();
      delayms(wait);

      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);        // turn every third pixel off
        }
      }
    }
    
}
//*********************************************************************************
// This one is correct
void MultiCometRev(unsigned char rred, unsigned char ggreen, unsigned char bblue, int skipt, int cycles, unsigned int wait) 
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)      
    {
    for (spacing = 0; spacing < skipt; spacing++) 
      {
                          // spacing here   \/ fills in gap at end of strip
      for (pixelpos = 0; pixelpos < strip.numPixels() + skipt; pixelpos = pixelpos + skipt)        
        {
           
        // "9 thru 1" changes how comet is pointed
        // pixelpos - spacing changes scroll direction
        strip.setPixelColor(pixelpos - spacing - 9 , rred, ggreen, bblue); 
        strip.setPixelColor(pixelpos - spacing - 8, rred / 2, ggreen / 2, bblue / 2);
        strip.setPixelColor(pixelpos - spacing - 7, rred / 4, ggreen / 4, bblue / 4);
        strip.setPixelColor(pixelpos - spacing - 6 , rred / 8, ggreen / 8, bblue / 8);
        strip.setPixelColor(pixelpos - spacing - 5, rred / 16, ggreen / 16, bblue / 16);
        strip.setPixelColor(pixelpos - spacing - 4, rred / 32, ggreen / 32, bblue / 32);
        strip.setPixelColor(pixelpos - spacing - 3, rred / 64, ggreen / 64, bblue / 64);
        strip.setPixelColor(pixelpos - spacing - 2, rred / 128, ggreen / 128, bblue / 128);
        strip.setPixelColor(pixelpos - spacing - 1, 0x000000);  
        }
      strip.show();
      delayms(wait);
      
      for (pixelpos = 0; pixelpos < strip.numPixels() + skipt; pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);        // turn every third pixel off
        }
      }
    }
    
}
//*********************************************************************************
void StaticCometF(unsigned char rred, unsigned char ggreen, unsigned char bblue, int skipt, int cycles, unsigned int wait) 
{
  int pixelpos, spacing, loops;
  
  for (loops = 0; loops < cycles; loops++)      
    {  
    for (spacing = 0; spacing < skipt; spacing++) 
      {
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        ReadEnc();
        // "1 thru 9" changes how comet is pointed  
        // i + q changes scroll direction
        strip.setPixelColor(pixelpos + spacing - 1 , rred, ggreen, bblue);                    
        strip.setPixelColor(pixelpos + spacing - 2, rred / 2, ggreen / 2, bblue / 2);     
        strip.setPixelColor(pixelpos + spacing - 3, rred / 4, ggreen / 4, bblue / 4);
        strip.setPixelColor(pixelpos + spacing - 4 , rred / 8, ggreen / 8, bblue / 8);
        strip.setPixelColor(pixelpos + spacing - 5, rred / 16, ggreen / 16, bblue / 16);
        strip.setPixelColor(pixelpos + spacing - 6, rred / 32, ggreen / 32, bblue / 32);
        strip.setPixelColor(pixelpos + spacing - 7, rred / 64, ggreen / 64, bblue / 64);
        strip.setPixelColor(pixelpos + spacing - 8, rred / 128, ggreen / 128, bblue / 128);
        strip.setPixelColor(pixelpos + spacing - 9, 0x000000);  
        }
      for (pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt) 
        {
        strip.setPixelColor(pixelpos + spacing, 0);        // turn every third pixel off
        }
      }
    }
    strip.show();
    delayms(wait);    
}
//****************************************************************************
void plotpixel(int pixelpos, char r, char g, char b, int wait)
{       
        strip.setPixelColor(pixelpos, r, g, b);                  
        strip.show();
        delayms(wait); 
}
//****************************************************************************
void streampixel(char r, char g, char b, int skipt, int wait)
{
  int pixelpos; 

    for(pixelpos = 1; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt)
        {           
        strip.setPixelColor(pixelpos, r, g, b);         
        strip.show();
        delayms(wait); 
        }
}
//****************************************************************************************
void colortwinkle(int wait, int skipt, int repeats)
{
  int times;
  for (times = 0; times < repeats; times++)
    {      
    FadePixel(random(num_of_pixels), pixelcolors[random(22)][3], pixelcolors[random(22)][3], pixelcolors[random(22)][3], 0, 0, 0, wait);
    strip.show(); 
    delayms(1);
    }
   
}
//****************************************************************************************
void FadeInOut(char cred, char cgreen, char cblue, int loops, int skipt, int wait)
{
  float r, g, b;
  int pixelpos;


  for(int i = 0; i < loops; i = i + 1) 
    {     
    for(int k = 0; k < 256; k = k + 1) 
      { 

      r = (k / 256.0) * cred; 
      g = (k / 256.0) * cgreen; 
      b = (k / 256.0)  *cblue; 
      for(pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt)
        {
        strip.setPixelColor(pixelpos, r, g, b); 
        strip.show(); 
        }
        delayms(wait);
      } 
    for(int k = 255; k >= 0; k = k - 2) 
      {
      r = (k / 256.0) * cred;
      g = (k / 256.0) * cgreen;
      b = (k / 256.0) * cblue;
      for(pixelpos = 0; pixelpos < strip.numPixels(); pixelpos = pixelpos + skipt)
        {
        strip.setPixelColor(pixelpos, r, g, b); 
        strip.show(); 
        }
        delayms(wait);
      }
    }
}
//****************************************************************************
void FadeStrip(char rrednext, char ggreennext, char bbluenext,
                char rredlast, char ggreenlast, char bbluelast,
                int spaces, int fadrate)
{
// ***Declare Temp Variables****************************
   float rlast, glast, blast, rcurr, gcurr, bcurr, rcalc, gcalc, bcalc;
   float MaxTemp, MaxStep, rtemp, gtemp, btemp;
   float rlasta, glasta, blasta;
   float rnow, gnow, bnow;
   unsigned long temp, temp1;
   unsigned int goFlag, nnn, pixelpos;
   unsigned long rred;
   unsigned int ggreen;
   unsigned char bblue;
   unsigned char checkEnc;
   
   pixelpos = 0;
   goFlag = 1;
   
   while(goFlag == 1)
      {
      rcurr =  rredlast;
      gcurr =  ggreenlast;
      bcurr =  bbluelast;
      rlast =  rrednext;
      glast =  ggreennext;
      blast =  bbluenext;
      rnow = rcurr - rlast;
      gnow = gcurr - glast;
      bnow = bcurr - blast;
      rlasta = abs(rnow);
      glasta = abs(gnow);
      blasta = abs(bnow);
      
      MaxTemp = max(rlasta, glasta);
      MaxStep = max(blasta, MaxTemp);
      rcalc = rlast;
      gcalc = glast;
      bcalc = blast;
     //*******************************************************
     // Calculate proportional fade rates
      for (nnn = 0; nnn < MaxStep; nnn++)
        {
        rtemp = (rcurr - rlast) / MaxStep;
        gtemp = (gcurr - glast) / MaxStep;
        btemp = (bcurr - blast) / MaxStep;
        rcalc = rcalc + rtemp;
        gcalc = gcalc + gtemp;
        bcalc = bcalc + btemp;
        rred = rcalc;
        ggreen = gcalc;
        bblue = bcalc;

     //*******************************************************
     // Populate Faded Colors
        for (pixelpos = 0; pixelpos < strip.numPixels() + 1; pixelpos = pixelpos + spaces)
            {
            strip.setPixelColor(pixelpos, rred, ggreen, bblue); 
            strip.show(); 
            }
     //*******************************************************
//      
     //if(checkEnc == 1) break;
     // Delay and Display Frame
        strip.show();     
        delayms(fadrate);
        } // End For MaxStep
        goFlag = 0;
    } // End While
}
//****************************************************************************
void FadePixel(int pos, char rrednext, char ggreennext, char bbluenext,
                char rredlast, char ggreenlast, char bbluelast,
                int fadrate)
{
// ***Declare Temp Variables****************************
   float rlast, glast, blast, rcurr, gcurr, bcurr, rcalc, gcalc, bcalc;
   float MaxTemp, MaxStep, rtemp, gtemp, btemp;
   float rlasta, glasta, blasta;
   float rnow, gnow, bnow;
   unsigned long temp, temp1;
   unsigned int goFlag, nnn, pixelpos;
   unsigned long rred;
   unsigned int ggreen;
   unsigned char bblue;
   
   pixelpos = 0;
   goFlag = 1;
   
   while(goFlag == 1)
      {
      rcurr =  rredlast;
      gcurr =  ggreenlast;
      bcurr =  bbluelast;
      rlast =  rrednext;
      glast =  ggreennext;
      blast =  bbluenext;
      rnow = rcurr - rlast;
      gnow = gcurr - glast;
      bnow = bcurr - blast;
      rlasta = abs(rnow);
      glasta = abs(gnow);
      blasta = abs(bnow);
      
      MaxTemp = max(rlasta, glasta);
      MaxStep = max(blasta, MaxTemp);
      rcalc = rlast;
      gcalc = glast;
      bcalc = blast;
     //*******************************************************
     // Calculate proportional fade rates
      for (nnn = 0; nnn < MaxStep; nnn++)
        {
        rtemp = (rcurr - rlast) / MaxStep;
        gtemp = (gcurr - glast) / MaxStep;
        btemp = (bcurr - blast) / MaxStep;
        rcalc = rcalc + rtemp;
        gcalc = gcalc + gtemp;
        bcalc = bcalc + btemp;
        rred = rcalc;
        ggreen = gcalc;
        bblue = bcalc;

     //*******************************************************
       strip.setPixelColor(pos, rred, ggreen, bblue);          
     //*******************************************************
     // Delay and Display Frame
        strip.show();     
        delayms(fadrate);
        } // End For MaxStep
        goFlag = 0;
    } // End While
}
//****************************************************************************
void FadePattern(int skipt, unsigned char rrednext, unsigned char ggreennext, unsigned char bbluenext,
                unsigned char rredlast, unsigned char ggreenlast, unsigned char bbluelast,
                int fadrate)
{
// ***Declare Temp Variables****************************
   float rlast, glast, blast, rcurr, gcurr, bcurr, rcalc, gcalc, bcalc;
   float MaxTemp, MaxStep, rtemp, gtemp, btemp;
   float rlasta, glasta, blasta;
   float rnow, gnow, bnow;
   unsigned long temp, temp1;
   unsigned int goFlag, nnn, pixelpos;
   unsigned long rred;
   unsigned int ggreen;
   unsigned char bblue;
   
   pixelpos = 0;
   goFlag = 1;
   
   while(goFlag == 1)
      {
      rcurr =  rredlast;
      gcurr =  ggreenlast;
      bcurr =  bbluelast;
      rlast =  rrednext;
      glast =  ggreennext;
      blast =  bbluenext;
      rnow = rcurr - rlast;
      gnow = gcurr - glast;
      bnow = bcurr - blast;
      rlasta = abs(rnow);
      glasta = abs(gnow);
      blasta = abs(bnow);
      
      MaxTemp = max(rlasta, glasta);
      MaxStep = max(blasta, MaxTemp);
      rcalc = rlast;
      gcalc = glast;
      bcalc = blast;
     //*******************************************************
     // Calculate proportional fade rates
      for (nnn = 0; nnn < MaxStep; nnn++)
        {
        rtemp = (rcurr - rlast) / MaxStep;
        gtemp = (gcurr - glast) / MaxStep;
        btemp = (bcurr - blast) / MaxStep;
        rcalc = rcalc + rtemp;
        gcalc = gcalc + gtemp;
        bcalc = bcalc + btemp;
        rred = rcalc;
        ggreen = gcalc;
        bblue = bcalc;

     //*******************************************************
     // Populate Faded Colors
        for (pixelpos = 0; pixelpos < strip.numPixels() + 1; pixelpos = pixelpos + skipt)
            {
            strip.setPixelColor(pixelpos, rred, ggreen, bblue); 
            strip.show(); 
            }       
     //*******************************************************
     // Delay and Display Frame
        //strip.show();     
        delayms(fadrate);
        } // End For MaxStep
        goFlag = 0;
    } // End While
}
//****************************************************************************
void FadeSection(int startpos, int endpos, char rrednext, char ggreennext, char bbluenext,
                char rredlast, char ggreenlast, char bbluelast,
                int fadrate)
{
// ***Declare Temp Variables****************************
   float rlast, glast, blast, rcurr, gcurr, bcurr, rcalc, gcalc, bcalc;
   float MaxTemp, MaxStep, rtemp, gtemp, btemp;
   float rlasta, glasta, blasta;
   float rnow, gnow, bnow;
   unsigned long temp, temp1;
   unsigned int goFlag, nnn, pixelpos;
   unsigned long rred;
   unsigned int ggreen;
   unsigned char bblue;
   
   pixelpos = 0;
   goFlag = 1;
   
   while(goFlag == 1)
      {
      rcurr =  rredlast;
      gcurr =  ggreenlast;
      bcurr =  bbluelast;
      rlast =  rrednext;
      glast =  ggreennext;
      blast =  bbluenext;
      rnow = rcurr - rlast;
      gnow = gcurr - glast;
      bnow = bcurr - blast;
      rlasta = abs(rnow);
      glasta = abs(gnow);
      blasta = abs(bnow);
      
      MaxTemp = max(rlasta, glasta);
      MaxStep = max(blasta, MaxTemp);
      rcalc = rlast;
      gcalc = glast;
      bcalc = blast;
     //*******************************************************
     // Calculate proportional fade rates
      for (nnn = 0; nnn < MaxStep; nnn++)
        {
        rtemp = (rcurr - rlast) / MaxStep;
        gtemp = (gcurr - glast) / MaxStep;
        btemp = (bcurr - blast) / MaxStep;
        rcalc = rcalc + rtemp;
        gcalc = gcalc + gtemp;
        bcalc = bcalc + btemp;
        rred = rcalc;
        ggreen = gcalc;
        bblue = bcalc;

     //*******************************************************
     // Populate Faded Colors
        for (pixelpos = startpos; pixelpos < endpos; ++pixelpos)
            {
            strip.setPixelColor(pixelpos, rred, ggreen, bblue);
            strip.show();  
            }       
     //*******************************************************

     // Delay and Display Frame
        //strip.show();     
        delayms(fadrate);
        } // End For MaxStep
        goFlag = 0;
    } // End While
}


//****************************************************************************
void colorFade(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) 
{
  for(uint16_t i = 0; i < strip.numPixels(); i++) 
      {
      uint8_t startR, startG, startB;
      uint32_t startColor = strip.getPixelColor(i); // get the current colour
      startB = startColor & 0xFF;
      startG = (startColor >> 8) & 0xFF;
      startR = (startColor >> 16) & 0xFF;  // separate into RGB components

      if ((startR != r) || (startG != g) || (startB != b))
        {  // while the curr color is not yet the target color
        if (startR < r) startR++; else if (startR > r) startR--;  // increment or decrement the old color values
        if (startG < g) startG++; else if (startG > g) startG--;
        if (startB < b) startB++; else if (startB > b) startB--;
        strip.setPixelColor(i, startR, startG, startB);  // set the color
        strip.show();
        delay(wait);  // add a delay if its too fast
        }
      delay(1000);
      }
}
