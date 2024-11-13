# StartASM

## Overview
StartASM is a personal project that aims to create a simple, beginner-friendly assembly language. Its main goals are:
- Create a readable, English-like syntax with greater verbosity and clarity
- Abstract many complicated facets of assembly programming to reduce complexity
- Introduce assembly programming concepts in a straightforward, simplified manner

StartASM was created after personally struggling to learn x86 as a complete newcomer to assembly programming. I want this project to be a simpler, more intuitive introduction to assembly concepts that could be an effective stepping stone toward learning industry-standard languages. It is also a personal exercise in self-learning and is my first time engaging with many of these concepts myself.

## Usage
Download the repository and open it in a code editor. This project uses Cmake, thus to build run the following in the terminal within the root folder:
```bash
mkdir build
cd build
cmake ..
make
```

Run the StartASM executable. Create a text file with StartASM code and place it into the root directory, then run the executable. StartASM uses the `.sasm` file extension. When running the executable and providing a filename, you can either include the `.sasm` extension or omit it, but don't append a different file extension (like .txt).

The compiler can be invoked via a CLI. Here are the possible options when envoking `./startasm --help`:
```
StartASM Compiler Usage:
  startasm compile <filepath.sasm> [options]
Options:
  --help        Display this help message and exit
  --timings     Print out timings for each compilation step
  --tree        Print out the AST (Abstract Syntax Tree)
  --ir        Print out generated LLVM IR
  --silent      Suppress output (except syntax errors)
  --truesilent  Suppress all output, including syntax errors
Note that the use of --silent or --truesilent will override output flags such as --tree and --timings.
```
You can also check the `examples` folder for examples. Each code file contains a comment explaining its purpose. There are included testing scripts available in the `testing` folder, including benchmarking and AST testing.

Also make sure to check out the `documentations` folder for more information about StartASM's features, syntax, and some examples! This is still very much a work-in-progress project, so updates will be continual.

## License
This project is licensed under the MIT license. Feel free to fork or contribute to this project or use it in any manner you like.