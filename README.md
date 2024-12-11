Sand Table Gcode Sender
 
This project is a mashup of several programs to create yet another version of a COREXY Sand Table.
My frustration was in the complexity of other projects, although done quite well,
but I wanted a simpler approach.  
Basic components are:

grbl cnc shield and Arduino UNO for motor controller.
TMC2209 Stepper Drivers for quiet operation.
ESP32 and SDCard module for sending gcode programs to the grbl motor controller.
STL Files from https://www.instructables.com/Easily-Build-a-MACHINE-THAT-DESTROYS-WHAT-IT-CREAT/  project.
The STL Files in this project folder were modified and used.

I started with a modified version of the original V1.1 grbl source code.
That source code is attached in this project.  All you need do is load it onto your Arduino UNO.
Mostly it is changed for IO config and a Baud rate of 9600 in order to keep up with
motor moves and grbl handshaking.

The sand patterns were created with https://github.com/markroland/sand-table-pattern-maker.
I modified the "env.js in the "SandPatternMaker525\assets\js" folder to match the 
dimensions of my sand table, roughly 525mm x 525mm.

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







