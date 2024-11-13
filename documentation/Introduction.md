# Introduction
### An introduction to the StartASM Language

## Features
StartASM is composed of three parts:
- An LLVM based compiler that translates .sasm straight to machine code
- A runtime library for special runtime safety checks and support
- A custom IDE (to be built soon)

The language is based on a load-store, and is fully turing complete. Here are the supported operations thus far:
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
- inputs and outputs (using registers)
- prints (Denoted with " ")
- labels (Denoted with ' ')
- comments (Denoted with " ")

One of the main features of StartASM is its abstraction to reduce the barrier of entry. This includes:
- Datatypes (ints, floats, bools, chars, addresses) that eliminate the need to interpret hex directly
- A dynamic type system with enforced type safety while allowing users to 'cast' data (interpret the byte sequence differently)
- Simplified memory model aligned to 32 bits for every datatype
- Simplified I/O and terminal

## Syntax
StartASM is designed to be as close to plain English as possible. The syntax trades away traditional opcode mnemonics used in most ASMs in favor of readable syntax closer to high-level languages. This includes the use of full-word instructions, transitional conjunctions to demonstrate operand relationships, and clearly denoted register (`r0`-`r9`), memory (`m<0>`-`m<999999999>`) and instruction memory (`i[0]`-`i[999999999]`) operands. For example:

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
- Provides transitional conjunction to show the relationship as an attribute of instruction `by`
- States explicitly the attribute operand `r2`

StartASM will enforce type safety as checked at runtime time. For example:

`add r1 with r2 to r3`

- Will throw an exception if r1 and r2 are not tracked to be of the same datatype
- Will cast r3 into the data type of the result
- Will perform the correct addition operation for the given datatypes (integer arithmetic, floating point arithmetic, etc.)

In summary:
- Transitional conjunctions are used to show operand relationships (except for source/self operands, which are implicit for a natural language feel). e.g. `from`, `to`, `with`, `by`. Conditionals are denoted by `if`
- Operands are stated explicitly and numerically. e.g. `r0`, `m<39>`
- Base instructions are stated in full, with all instruction conditions also stated in full (no mnemonics). e.g. `move`, `shift left`
- Types are stated and enforced but flexible in allowing reinterpretations of the same byte value

Here are all possible instruction combinations as of now:
- `move (register) to (register)`
- `load (memory/register) to (register)`
- `store (register) to (memory/register)`
- `create integer/float/boolean/character/memory/instruction (value) to (register)`
- `cast integer/float/boolean/character/memory/instruction (register)`
- `add (register) with (register) to (register)`
- `sub (register) with (register) to (register)`
- `multiply (register) with (register) to (register)`
- `divide (register) with (register) to (register)`
- `or (register) with (register)`
- `and (register) with (register)`
- `not (register)`
- `shift left/right (register) by (register)`
- `compare (register) with (register)`
- `jump if (unconditional/greater/less/equal/zero/unequal/nonzero) to (instruction/label/register)`
- `call to (instruction/label/register)`
- `push (register)`
- `pop to (register)`
- `return`
- `stop`
- `input integer/float/boolean/character to (register)`
- `output (register)`
- `print ("comment"/newline)`
- `label '(label)'`
- `comment "(comment)"`

Where:
- Registers are `r0`-`r9`
- Memory addresses are `m<0>` to `m<999999999>`, with each value being 4 bytes in program memory
- Instruction addresses are `i[0] to i[999999999]`, with each value being a 4 byte instruction
- Labels are any word within single quotes *'likeThis'*
- Comments are any string within double quotes *"Like this"*

Here are some other details of note:
- StartASM interacts directly in the terminal akin to higher-level languages. Methods `input` and `output` work strictly with dynamic data stored in registers, whereas there's a seperate `print` statement to allow easy user prompts and debugging. All outputs and inputs are on the same line unless expressly preceded by a `print newline`.
- StartASM also supports manipulating instruction and memory addresses, alowing basic pointer functionality and operations. This can be useful for creating contiguous data structures (such as arrays) or jump tables. Thus, address instructions such as `load`, `store`, `jump` and `call` allow both immediates and registers holding valid addresses (though this will be checked for type safety).

## Technologies
StartASM is, as of now, fully developed in C++. The compiler is built using C++ and an LLVM backend. This project also uses OpenMP multithreading to improve performance.