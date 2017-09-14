# MindMeld
MindMeld is a derivative of BrainF**k that uses two data pointers, rather than just one.  
Download the lastest release [here!](https://github.com/quietsamurai98/MindMeld/releases)

## How does it work?
Each MindMeld command has two characters, the instruction and the pointer.  
The instructions are identical those of standard BrainF**k, except each command is followed by the data pointer that the command will use. For example, the command to print the byte at data pointer A would be `.A`, while the command to do the same with data pointer B would be `.B`.

Example: Adding two bytes given by the user using MindMeld
```
,A		//read cin into cell 0
>A,A		//read cin into cell 1

<A		//set data pointer a to cell 0
>B>B		//set data pointer b to cell 2

[A		//add cell 0 to cell 2
	-A
	+B
]A

>A		//set data pointer a to cell 1
[A		//add cell 1 to cell 2
	-A
	+B
]A

.B		//print cell 2
```

## Technical Details and Miscallaneous Information
 * Comments must not contain any of the following characters: `+-<>.,AB`
 * Loop Logic
   * Opening and closing brackets do not require matching pointers.
   * To enter a loop, only the opening bracket pointer must be non-zero.
   * To continue a loop, both the opening and closing bracket pointers must be non-zero.
 * Individual Memory Cells
   * Each memory cell is an unsigned char, with range 0-255.
   * If incrementing/decrementing a memory cell causes an overflow/underflow, the cell's value will wrap around to 0/255, all respectively.
 * The Memory Tape
   * The memory tape contains 30000 cells, and does not wrap around.
   * Both data pointers are initialized starting at memory cell 0.
   * Attempting to access negative memory cells or memory cells greater than 30000 causes undefined behavior.
 * Other Quirks
   * The interpreter reads the source code from a file, strips out comments, and stores it in a char array. As a result, you will never be able to run any source code that is greater than 16 exbibytes, even hypothetically, and realistically, you cannot run any source code that is larger than the largest consecutive block of free RAM.

## Compilation Information and Usage
This program was created using Code::Blocks and the MinGW 4.3.0 compiler (gcc 7.1.0) on a Windows 10 computer. It also uses the conio.h header, so it likely will need some tweaking to work properly on UNIX systems.  
To use the interpreter program, simply run the executable and enter the path to your MindMeld source code file. The path can be either be absolute or relative, and it is allowed to contain spaces.
