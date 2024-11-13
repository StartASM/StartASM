# THIS IS INCOMPLETE - WILL BE UPDATED
## Progress
**Please note that StartASM is still in early development, so many critical features have yet to be implemented. This is a personal project by a student and, as such, is being developed incrementally.**

*Last updated: December 2, 2023*

**State:** StartASM currently consists of a compiler that validates the grammar, including syntax, symbols and semantics. Currently missing features of the compiler include scope checking and code generation.

**Progress:** Currently working on implementing scope checking and code generation.

**Current Goal:** Finish the compiler by the end of the year.


## Last Major Milestone
**From December 1, 2023**

*Reworked Lexer, Parser and AST | Value Creation, Type Systems and I/O | Performance Overhaul*

The entire StartASM compiler was fully upended, moving from regex-based parsing logic to recursive descent. I've implemented numerous changes, including abolishing the InstructionSet.cpp class with a new Parser.cpp class that more closely aligns with industry-standard practices. Created a new ParseTree class to store syntactic information and completely reworked the AST class to represent StartASM syntax better. I also instituted a significant overhaul to the lexer, which now categorizes tokens based on enum constants. Furthermore, I've removed problematic OpenMP parallelized sections (particularly those with unstable access/writing in non-thread-safe containers) while ensuring that currently parallelized segments are thread-safe. SIGNIFICANT performance increases can be found (up to 390x); here is a sample:

Regex (single threaded):
Time taken: 13.000235 seconds

Regex (multi-threaded):
Time taken: 3.149544 seconds

Recursive descent:
Time taken: 0.134222 seconds

[For 10000 lines of incorrect code]

Significant README updates and the introduction of major features, including the "create", "cast", "output", "input" and "print" methods, have been added. StartASM now contains critical features such as the creation of constant values, a fleshed-out type system, and I/O mechanisms. This is one of the most significant milestones in StartASM's development.