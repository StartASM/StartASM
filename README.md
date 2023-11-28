# StartASM

## Overview
StartASM is a personal project that aims to create a simple, beginner friendly assembly language. Its main goals are:
- Create a readable, English-like syntax with greater verbosity and clarity
- Abstract many complicated facets of assembly programming to reduce complexity
- Introduce assembly programming concepts in a straightforward, simplfied manner

StartASM was created after personally struggling learning x86 as a complete newcomer to assembly programming. I want this project to be a simpler, more intuitive introduction to assembly concepts that could act as an effective stepping stone towards learning industry standard languages. It is also a personal excercise in self learning, and is my first time engaging with many of these concepts myself.


## Features
StartASM will be composed of three parts:
- A custom compiler, written in C++, that validates and compiles StartASM code into intermediate instruction objects
- A full 32 bit simulator and runtime environment written in C++ - complete with registers and memory data structures - capable of executing StartASM instructions
- A custom IDE - built using Electron - to provide a visual development environment

The language is based on a load-store model, with seperate program and insruction memory (Harvard) in a big-endian layout. The runtime environment will also feature 10 registers, 32 bit addressable memory and instruction pools as well as a simple stack. The following operations will be supported:
- move (between registers)
- load (memory to register)
- store (register to memory)
- create (value to register)
- cast (register)
- arithmetic (add, sub, multiply, divide)
- bitwise operations (and, or, not)
- bitwise shift (left/right)
- comparison (between registers, numerical)
- jump (conditional and unconditional)
- call (unconditional)
- stack manipulation (push and pop)
- labels (Denoted with ' ')
- comments (Denoted with " ")

One of the main features of StartASM is its abstraction to reduce barrier of entry. This includes:
- Datatypes (ints, floats, bools, chars, addresses) that eliminates the need to interpret hex directly
- A static type system with enforced type safety while allowing users to 'cast' data (interpret the byte sequence differently) 
- Simplified memory model aligned to 32 bits for every datatype


## Syntax
StartASM is designed to be as close to plain English as possible. The syntax trades away traditional opcode mnemonics used in most ASM's in favor of readable syntax closer to high-level languages. This includes the use of full-word instructions, transitional conjunctions to demonstrate operand relationships, and clearly denoted register (`r0`-`r9`), memory (`m<0>`-`m<999999999>`) and instruction memory (`i[0]`-`i[999999999]`) operands. For example:

`move r1 to r2`
- Denotes the instruction first `move`
- States implicit source operand `r1`
- Provides a transitional conjunction to show where data is flowing `to`
- States explicitly destination operand `r2`

For a more complex example:

`shift right r1 with r2`
- Denotes the instruction first `shift`
- Provides direction after the base instruction `right`
- States implicitly self operand `r1`
- Provides a transitional conjunction to show relationship as attribute of instruction `by`
- Sates explicitly the attribute operand `r2`

StartASM will enforce type safety as checked at compile time. For example:

`add r1 with r2 to r3`

- Will throw an exception if r1 and r2 are not tracked to be of the same datatype
- Will cast r3 into the datatype of the result
- Will perform the correct addition operation for the given datatypes (integer arithmetic, floating point arithmetic, etc.)

In summary:
- Transitional conjunctions are used to show operand relationships (except for source/self operands, in which it is implicit for a natural language feel). e.g. `from`, `to`, `with`, `by`. Conditionals are denoted by `if`
- Operands are stated explicitly and numerically. e.g. `r0`, `m<39>`
- Base instructions are stated in full, with all instruction conditions also stated in full (no mnemonics). e.g. `move`, `shift left logically`
- Types are stated and enforced, but flexibile in allowing reinterpretations of the same byte value


## Usage
Download the repository and open in a code editor. Run the StartASM executable, and ensure a C++ compiler that supports OpenMP is installed (preferably g++). Create a text file with StartASM code and place it into the 'code' folder, then run the executable. When running the executable and providng a filename, omit the `.txt` part. `ExampleCode.txt` should become `ExampleCode`.

Here are all possible instruction combinations as of now:
- `move (register) to (register)`
- `load (memory) to (register)`
- `store (register) to (memory)`
- `create integer/float/boolean/character (value) to (register)`
- `cast integer/float/boolean/character/address (register)`
- `add (register) with (register) to (register)`
- `sub (register) with (register) to (register)`
- `multiply (register) with (register) to (register)`
- `divide (register) with (register) to (register)`
- `or (register) with (register)`
- `and (register) with (register)`
- `not (register)`
- `shift left/right (register) by (register)`
- `compare (register) with (register)`
- `jump if (unconditional/greater/less/equal/zero/unequal/nonzero) to (instruction/label)`
- `call to (instruction/label)`
- `push (register)`
- `pop to (register)`
- `return`
- `stop`
- `label '(label)'`
- `comment "(comment)"`

Where: 
- Registers are `r0`-`r9`
- Memory addresses are `m<0>` to `m<999999999>`, with each value being 4 bytes in program memory
- Instruction addresses are `i[0] to i[999999999]`, with each value being a 4 byte instruction
- Labels are any word within single quotes *'likeThis'*
- Comments are any string within double quotes *"Like this"*

Check the 'code' folder for examples. 'ExampleCode' is a file that provides no syntax errors, whereas the other files are test cases for the compiler. The 'Long' examples are for benchmarking the compiler and it's efficiency/ Here is a general example of valid StartASM code:

```
comment "StartASM!"

load m<435> to r2
move r2 to r1
store r1 to m<0234>
or r1 with r2
shift right r2 by r1
not r1
compare r2 with r1
jump if greater to i[0]
jump if unconditional to 'overHere!'
add r1 with r2 to r3
divide r3 with r2 to r1
pop to r3
stop

label 'overHere!'
stop

comment "EndASM! get it? haha"
```
The compiler will also return syntax and symbol/scope error messages that show the excepted line, token and what the compiler expected if applicable. For example, submitting this invalid code:
``` 
shift right r2 with r1
junk r1
move m<1> to r6
```

Will return the following errors:
```
Invalid syntax at line 1: 'shift right r2 with r1'
Unknown conjunction 'with'. Expected 'by'

Invalid syntax at line 2: 'junk r1'
Unknown instruction 'junk'

Invalid syntax at line 3: 'move m<1> to r6'
Unknown source 'm<1>'. Expected register r0-r9
```

## Technologies
StartASM is, as of now, fully developed in C++. The intent is for the compiler and runtime environment to be built using C++, while the front end will be built using Electron and node.js. This project also uses OpenMP multithreading to improve performance.

## Progress
**Please note that StartASM is still in early development, and as such many critical features are yet to be implemented. This is a personal project by a student, and as such is being developed incrementally.**

*Last updated: November 28, 2023*

**State:** StartASM currently only consists of a compiler that validates syntax through recursive descent parsing and builds a parse tree. Current focus is on implementing effective symbol resolution and semantic analysis, hopefully finishing the compiler by year's end.

**Progress:** Currently working on implementing semantic analysis and symbol resolution, building out an AST.

**Current Goal:** Finish the compiler by the end of the year.

## Contact
If you want to contact me about this project, feel free to send an email to tahsinkalkie[at]gmail[dot]com