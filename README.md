# StartASM

## Overview
StartASM is a personal project that aims to create a simple, beginner friendly assembly language. Its main goals are:
- Create a readable, English-like syntax with greater verbosity and clarity
- Abstract many complicated facets of assembly programming to reduce complexity
- Introduce assembly programming concepts in a straightforward, simplfied manner

StartASM was created after personally struggling learning x86 as a complete newcomer to assembly programming. I want this project to be a simpler, more intuitive introduction to assembly concepts that could act as an effective stepping stone towards learning industry standard languages. It is also a personal excercise in self learning, and is my first time engaging with many of these concepts myself.


## Features
StartASM will be composed of three parts:
- A custom compiler, written in C++, that validates and compiles StartASM code into intermediate instructions
- A full simulator and runtime environment written in C++, complete with registers and memory data structures, capable of executing StartASM instructions
- A custom IDE, built using Electron, to provide a visual development environment

The language is based on a load-store model, with seperate program and insruction memory (Harvard) in a big-endian layout. The runtime environment will also feature 10 registers, up to 1 MB of both program and instruction memory as well as a simple stack. The following operations will be supported:
- move (between registers)
- load (data/memory to register)
- store (register to memory)
- arithmetic (add, sub, multiply, divide)
- bitwise operations (and, or, not)
- bitwise shift (left/right, logical/arithmetic)
- jump (conditional and unconditional)
- stack manipulation (push and pop)
- comments (Denoted with "")

One of the main features of StartASM is its abstraction to reduce barrier of entry. This includes:
- Ability to cast data rather than working with raw hex
- Automatically handled dyamic memory allocation
- Simplified data and storage models


## Syntax
StartASM is designed to be as close to plain English as possible. The syntax trades away traditional opcode mnemonics used in most ASM's in favor of readable syntax closer to high-level languages. This includes the use of full-word instructions, transitional conjunctions to demonstrate operand relationships, and clearly denoted register (r0-r9), memory (m<000000>) and instruction memory (i[000000]) operands. For example:

`move r1 to r2`
- Denotes the instruction first (move)
- States implicit source operand (r1)
- Provides a transitional conjunction to show where data is flowing (to)
- States explicitly destination operand (r2)

For a more complex example:

`shift right logically r1 with r2`
- Denotes the instruction first (shift)
- Provides direction and type, always after the base instruction (right logically)
- States implicitly self operand (r1)
- Provides a transitional conjunction to show relationship (by, as in attribute for the instruction)
- Sates explicitly the attribute operand (r2)

In summary:
- Transitional conjunctions are used to show operand relationships (except for source/self operands, in which it is implicit for a natural language feel). e.g. from, to, with, by, if for condittionals
- Operands are stated explicitly and numerically
- Base instructions are stated in full, with all instruction conditions also stated in full (no mnemonics)


## Usage
Download the repository and run the StartASM executable. Create a text file with StartASM code and place it into the 'code' folder, then run the executable. Here are all possible instruction combinations as of now:

- move (register) to (register)
- load (memory/value) to (register)
- store (register) to (memory)
- add (register) with (register) to (register)
- sub (register) with (register) to (register)
- multiply (register) with (register) to (register)
- divide (register) with (register) to (register)
- or (register) with (register)
- and (register) with (register)
- not (register)`
- shift left arithmetically (register) by (register)
- shift left logically (register) by (register)
- shift right arithmetically (register) by (register)
- shift right logically (register) by (register)
- jump unconditionally to (instruction)
- jump if (greater/less/equal/zero) to (instruction)
- jump if not (equal/zero) to (instruction)
- push (register)
- pop to (register)
- stop
- comment "(comment)"

Where: 
- Registers are r0-r9
- Memory addresses are m<000000> to m<999999>, with each value being a byte (can be variable length)
- Instruction addresses are i[000000] to i[999999], with each value being an instruction (can be variable length)
- Comments are any string within double quotes *"Like this"*

Check the 'code' folder for examples. 'ExampleCode' is a file that provides no syntax errors, whereas the other files are test cases for the compiler. Here is that code found in 'ExampleCode':

```
move r1 to r2
load 8 to r5
load m<435> to r2
store r6 to m<0234>
add r1 with r2 to r2
sub r6 with r4 to r2
multiply r5 with r4 to r6
divide r8 with r8 to r1
or r4 with r2
and r1 with r2
not r6
shift left arithmetically r4 by r1
shift left logically r8 by r2
shift right arithmetically r2 by r4
shift right logically r2 by r6
jump unconditionally to i[3]
jump if greater to i[8]
jump if less to i[0]
jump if equal to i[11]
jump if not equal to i[0032149]
jump if zero to i[7]
jump if not zero to i[8]
push r8
pop to r6
stop
comment "This is a comment"
```


## Technologies
StartASM is, as of now, fully developed in C++. The intent is for the compiler and runtime environment to be built using C++, while the front end will be built using Electron and node.js. This project also uses OpenMP multithreading to improve performance.

## Progress
*Last updated: Tuesday, November 7, 2023*

**State:** StartASM currently only consists of a compiler that checks for syntax. The provided program will tell the user whether the given code adheres to the language's syntax. 

**Progress:** Currently working on implementing the compilation method to turn valid syntax into an AST (the class for which is complete), which will then turn into instruction objects for the runtime environment.

**Current Goal:** Finish the compiler by the end of the year.

## Contact
If you want to contact me about this project, feel free to send an email to tahsinkalkie@gmail.com