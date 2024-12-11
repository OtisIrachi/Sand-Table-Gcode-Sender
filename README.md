Sand Table Gcode Sender
 
This project is a mashup of several programs to create yet another version of a COREXY Sand Table.
My frustration was in the complexity of other projects, although done quite well,
but I wanted a simpler approach.  
I started with a modified version of the original V1.1 grbl source code.
Mostly it is changed for IO config and a Baud rate of 9600 in order to keep up with
motor moves and grbl handshaking.

The sand patterns were created with https://github.com/markroland/sand-table-pattern-maker.
I modified the "env.js in the "SandPatternMaker525\assets\js" folder to match the 
dimensions of my sand table, roughly 525mm x 525mm.
Example:

//********************************************************************
var env = {
    
	"table" : {
        
		"format": "cartesian",
        
		"units": "mm",
        
		"x": {
            
		     "min": 5.0,
            
		     "max": 520.0
        
		},
        
		"y": {
            
		     "min": 5.0,
            
		     "max": 520.0
        
		}
    
	},
    
	"motor": {
        
		"speed": 3000.0
    
	},
    
	"ball": {
        
		"diameter": 19.0
    
	},
    
	"gcode": {
        
		"command": "G1"
    
	},
    
	"recalculate_pattern": true

}
//********************************************************************

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







