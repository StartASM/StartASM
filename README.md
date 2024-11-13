# StartASM Compiler

## Overview
The following repository contains the main compiler for the StartASM language. It is the heart of the language implementation and is required for other modules of the project such as Ignition. It's implemented in C++, with an LLVM backend, and optimized for multithreading with OpenMP.

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
  startmasm ast <filepath.sasm> [options]
Options:
  --help        Display this help message and exit
  --timings     Print out timings for each compilation step
  --ir        Print out generated LLVM IR
  --silent      Suppress output (except syntax errors)
  --truesilent  Suppress all output, including syntax errors
Note that the use of --silent or --truesilent will override output flags such as --timings.
```
You can also check the `examples` folder for examples. Each code file contains a comment explaining its purpose. There are included testing scripts available in the `testing` folder, including benchmarking and AST testing.

Also make sure to check out the `documentations` folder for more information about StartASM's features, syntax, and some examples! This is still very much a work-in-progress project, so updates will be on the way.

## License
This project is licensed under the MIT license. Feel free to fork or contribute to this project or use it in any manner you like.
