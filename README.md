Sand Table Gcode Sender
 
This project is a mashup of several programs to create yet another version of a COREXY Sand Table.
My frustration was in the complexity of other projects, although done quite well,
but I wanted a simpler approach.  

Basic components are:

Grbl CNC Shield and Arduino UNO for stepper motor controller.

TMC2209 Stepper Drivers for quiet operation.

ESP32 and SDCard module for sending gcode programs to the grbl stepper motor controller.

STL Files referenced from https://www.instructables.com/Easily-Build-a-MACHINE-THAT-DESTROYS-WHAT-IT-CREAT/  project.

The STL Files in this project folder were modified and used here:  https://github.com/OtisIrachi/Sand-Table-Gcode-Sender/tree/main/Kinetic%20STL%20Files

I started with a modified version of the original V1.1 grbl source code.
That source code is attached in this project.  All you need do is load it onto your Arduino UNO.
Mostly it is changed for IO config and a Baud rate of 9600 in order to keep up with
motor moves and grbl handshaking.

The sand patterns were created with [https://github.com/markroland/sand-table-pattern-maker.](https://github.com/OtisIrachi/Sand-Table-Gcode-Sender/tree/main/SandPatternMaker525)
I modified the "env.js in the "SandPatternMaker525\assets\js" folder to match the 
dimensions of my sand table, roughly 525mm x 525mm.
Run the SandPatternMaker525 program by clicking on the "index.html" file.  It runs in your browser.
https://github.com/OtisIrachi/Sand-Table-Gcode-Sender/blob/main/SandPatternMaker525/index.html

The .gcode files are manually edited at beginning and the end of each 
file for simplicity in the arduino coding. All comments created by Sand Table Maker
are deleted and an Fxxxx speed command is added to the first G1 line, and an "M2"
command at the end of each gcode file.
ex: 
G1 X5.000 Y5.000 F6000
G1 X12.728 Y0.000
G1 X0.000 Y12.728
G1 X0.000 Y25.456.....
M2

The LED Strip Controller is here https://github.com/OtisIrachi/Sand-Table-Gcode-Sender/tree/main/ESPEncoderNeopixel.
Uses an ESP8266 D1 Mini Module,  a 30 LED/meter strip, and a Rotary Encoder Module with Push Button.

Wiring and Schematics found here: https://github.com/OtisIrachi/Sand-Table-Gcode-Sender/tree/main/Schematics





