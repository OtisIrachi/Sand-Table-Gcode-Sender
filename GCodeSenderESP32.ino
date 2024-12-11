//***********************************************************************************
// GCodeSenderESP32.ino
//
// ESP32 Sender:
// This program uses an ESP32 to transmit serial Gcode commands to a standard UNO CNC 
// Shield at 9600 Baud.  I/O consists of a micro SD Card SPI interface, a Start PB, 
// Pause PB, Next PB and Reset PB.  A 4 Channel Logic Level Converter board (Amazon)
// is needed to interface the 3.3V ESP32 RX and TX to the 5V UNO TX and RX. A schematic
// is provided with the project.
//
// GRBL and Sand Table:
// See "Build a Sisyphus MAGIC SAND Coffee Table - DIY Machines" on YouTube 
// for an excellent construction tutorial.
// The CNC Shield is connected to 2 Stepper Motors in a CoreXY Configuration.
// The "grbl" library was modified for 9600 Baud because of the exposed 
// TX and RX wires between the ESP32 and UNO. During contruction, twist 
// the TX and RX wires for added noise immunity.
// Several changes were made to "config.h" and "defaults.h" folders in grbl, 
// therefore use the supplied "grbl" folder for your cnc Shield UNO firmware.
// 
// Sand Patterns:
// Sand Table patterns were created with Sand Table Pattern Maker
// https://github.com/markroland/sand-table-pattern-maker
// Within this project are attached the modified folders for Sand Table Pattern Maker.
// See "SandPatternMaker325" and "SandPatternMaker525".
// You will notice in the \SandPatternMaker525\assets\js folder that env.js has been
// modified for this project.  xmax, ymax, motor speed, ball diameter, and gcode command.
// Changing these variables will customize your Sand Table designs for the size of your project.
// The examples provided are for 325x325mm square and 525x525mm square CoreXY Tables.
// To run the modified folder versions, click on the "index.html" file 
// in the \SandPatternMaker525 or \SandPatternMaker325 directories.
// After you download your newly created pattern, you must modify the .gcode file.
// At the beginning of the file, delate all lines starting with ;
// After the first G1 X--- Y--- line add a space and F2000 (or other speed number)
// to control your pattern speed.
// At the end of the .gcode file, delete the ; line and add an M2 to the last line of the file.
// Change the "const char* gcodeFiles[]" below to match your file listing.
//
// by RCI
// Handshaking programming assisted using ChatGPT.
// 11-22-2024
//***********************************************************************************

#include <SD.h>
#include <HardwareSerial.h>

// Pin definitions
#define PAUSE_BUTTON_PIN 25
#define START_BUTTON_PIN 27
#define NEXT_BUTTON_PIN  32
#define SD_CS_PIN 5

// Serial communication for GRBL
HardwareSerial GRBL(1);
#define GRBL_RX_PIN 16
#define GRBL_TX_PIN 17

// *****  my SD Card Filenames  *****
// ***** Yours will be different *****
// EraseDiagonal325.gcode
// FivePointStar325.gcode
// Gosper325.gcode
// HilbertCurve325.gcode
// PlainCircle325.gcode
// SandTable325.gcode
// SmoothSpiral325.gcode
// Spiral325.gcode
// Spirograph325.gcode
// SquareFrame325.gcode
// TwistedSixPoint325.gcode

// G-code file handling
const char* gcodeFiles[] = {"/SquareFrame325.gcode", "/SmoothSpiral325.gcode", "/Gosper325.gcode", "/HilbertCurve325.gcode", 
                            "/FivePointStar325.gcode", "/Spiral325.gcode"};
File gcodeFile;
int fileIndex = 0;

// Flags and states
bool paused = 0;
bool grblReady = 1;
//***********************************************************************************
// Prompt the user and wait for the start button
void promptAndWaitForStart(const char *message) 
{
    Serial.println(message);
    while (digitalRead(START_BUTTON_PIN) == HIGH) 
       {
       delay(50); 
       }
    delay(500); // Debounce delay
}
//***********************************************************************************
// Perform GRBL homing sequence
void performHoming() 
{
    sendGcode("$H"); // Send homing command
    waitForOk();
    Serial.println("Homing is complete.");
}
//***********************************************************************************
// Send the next G-code file
void sendNextGcodeFile() 
{
    if (fileIndex < sizeof(gcodeFiles) / sizeof(gcodeFiles[0])) 
       {
       gcodeFile = SD.open(gcodeFiles[fileIndex]);
       Serial.print("gcodeFile: "); Serial.println(gcodeFile);
       Serial.print("fileIndex: "); Serial.println(fileIndex);
       Serial.println("");
       if (gcodeFile) 
          {
          Serial.print("Opened: ");
          Serial.println(gcodeFiles[fileIndex]);
          fileIndex++;
          } 
        else 
           {
           Serial.println("Failed to open G-code file.");
           }
        } 
     else 
        {
        Serial.println("All G-code files processed.");
        Serial.println("Do all over again.");
        fileIndex = 0; // Reset to the first file if looping is desired
        paused = 0; 
        handleGcodeExecution();       
        }
}
//***********************************************************************************
// Handle G-code execution
void handleGcodeExecution() 
{
    static String line = "";

    if (gcodeFile && gcodeFile.available()) 
       {
       line = gcodeFile.readStringUntil('\n');
       line.trim();

       if (line.length() > 0 && line[0] != '(') // Skip comments and empty lines
         { 
         if (isGrblReady()) 
            {
            sendGcode(line.c_str());
            }
         }
       } 
    else if (gcodeFile) 
       {
       gcodeFile.close();
       sendNextGcodeFile();
       }
}
//***********************************************************************************
// Check if GRBL is ready
bool isGrblReady() 
{
    GRBL.print("?");
    delay(100); // Allow GRBL to respond
    while (GRBL.available()) 
       {
        String response = GRBL.readStringUntil('\n');
        if (response.startsWith("<Idle") || response.startsWith("<Run")) 
           {
           grblReady = true;
           return true;
           } 
        else if (response.indexOf("error") != -1 || response.startsWith("ALARM")) 
           {
           Serial.println("GRBL Error: " + response);
           grblReady = false;
           return false;
           }
       }
    return false;
}
//***********************************************************************************
// Send a single G-code command only to GRBL
void sendGcodeOnly(const char *command) 
{
    Serial.print("Sending: ");
    Serial.println(command);
    GRBL.println(command);

}
//***********************************************************************************
// Send a single G-code command to GRBL and wait for OK
void sendGcode(const char *command) 
{
    Serial.print("Sending: ");
    Serial.println(command);
    GRBL.println(command);
    waitForOk();
}
//***********************************************************************************
// Wait for "ok" from GRBL
void waitForOk() 
{
    while (true) 
       {
       if (GRBL.find("ok")) 
          {
          break;
          }
       }
}
//***********************************************************************************
// Handle the pause button
void handlePauseButton() 
{
    //************************************************
    if (digitalRead(PAUSE_BUTTON_PIN) == LOW) 
       {
       while (digitalRead(PAUSE_BUTTON_PIN) == LOW){} 
       paused = 1;
       sendGcode("!");
       Serial.println("Program Paused");
       }
    //************************************************   
    if (digitalRead(START_BUTTON_PIN) == LOW) 
       {
       while (digitalRead(START_BUTTON_PIN) == LOW){} 
       paused = 0;
       sendGcode("~");
       Serial.println("Program Resumed");
       }
    //************************************************   
 
}
//***********************************************************************************
void setup() 
{
    // Init serial communication
    Serial.begin(115200); 
    GRBL.begin(9600, SERIAL_8N1, GRBL_RX_PIN, GRBL_TX_PIN);
    delay(5000);

    Serial.println("GCodeSenderESP32.ino");
    delay(2000);
    
    // Init buttons
    pinMode(START_BUTTON_PIN, INPUT_PULLUP);
    pinMode(PAUSE_BUTTON_PIN, INPUT_PULLUP);  
    pinMode(NEXT_BUTTON_PIN,  INPUT_PULLUP);
    
    // Init SD card
    if (!SD.begin(SD_CS_PIN)) 
       {
       Serial.println("SD Card initialization failed!");
       while (true);
       }
    Serial.println("SD Card initialized.");
    delay(2000); 

    // Startup sequence
    promptAndWaitForStart("Press Start PB to Home");
    performHoming();

    //promptAndWaitForStart("Program is Ready to Start");
    sendNextGcodeFile();
}
//***********************************************************************************
void loop() 
{
    handlePauseButton();

    //************************************************   
    if (digitalRead(NEXT_BUTTON_PIN) == LOW) 
       {
       while (digitalRead(NEXT_BUTTON_PIN) == LOW){} 

       sendGcodeOnly("M2");       // GRBL Program End
       Serial.println("Grbl M2...");
       delay(2000);          
      
       GRBL.write(24);    // CTRL-X, Soft Reset
       waitForOk();       
       Serial.println("Grbl Reset...");
       delay(5000);
       
       Serial.println("Closing Gcode File");      
       gcodeFile.close();
       delay(5000);     

       performHoming();
       waitForOk();
       
       Serial.println("Loading next Gcode File");
       sendNextGcodeFile();
       }
    //************************************************
    
    if (!paused) 
       {
       handleGcodeExecution();
       }
       
}
//***********************************************************************************
//***********************************************************************************
