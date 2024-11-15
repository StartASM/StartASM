# StartASM Compiler

## Overview
The following repository contains the main compiler for the StartASM language. It is the heart of the language implementation and is required for other modules of the project such as Ignition. It's implemented in C++, with an LLVM backend, and optimized for multithreading with OpenMP.

## Usage
### Building and Running with Docker
The StartASM compiler can be built on your local machine with Docker. To do so, follow these steps:

1. **Install Docker**:
Make sure Docker is installed on your machine. You can download and install Docker from the Docker website or through a package manager.
For most Debian Linux-based systems, you can typically install it with:
```bash
sudo apt update
sudo apt install docker.io
```

2. **Build the Docker Image**:
Clone this repository and navigate to the project directory. Then build the Docker image:
```
docker build -t startasm .
```

3. **Run the Compiler**:
Invoke the compiler inside the Docker container:
```
docker run --rm -it startasm compile <filepath.sasm> [options]
```

For example, to compile a file:
```
docker run --rm -it startasm compile examples/ExampleCode.sasm
```
To bring up more info, you can invoke the `--help` flag, the following output of which can be seen here:
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
