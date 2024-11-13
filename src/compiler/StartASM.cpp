#include "compiler/Compiler.h"
#include <iostream>
#include <omp.h>
#include <string>
#include <algorithm>

//Include the Easter egg functions
#include "misc/.Secrets.h"

using namespace std;

//Function to check if a command-line option exists
bool cmdOptionExists(char** begin, char** end, const string& option) {
    return find(begin, end, option) != end;
}

//Function to check for valid .sasm file extension
bool isValidSASMFile(const string& filename) {
    if (filename.length() >= 5) {
        return filename.substr(filename.length() - 5) == ".sasm";
    }
    return false;
}

void displayHelp() {
    string title = R"(
       _____ _             _             _____ __  __
      / ____| |           | |     /\    / ____|  \/  |
     | (___ | |_ __ _ _ __| |_   /  \  | (___ | \  / |
      \___ \| __/ _` | '__| __| / /\ \  \___ \| |\/| |
      ____) | || (_| | |  | |_ / ____ \ ____) | |  | |
     |_____/ \__\__,_|_|   \__/_/    \_\_____/|_|  |_|
    )";
    cout << title << endl;
    cout << "StartASM Compiler Usage:" << endl;
    cout << "  startasm compile <filepath.sasm> [options]" << endl;
    cout << "  startasm ast <filepath.sasm> [options]" << endl;
    cout << "Options:" << endl;
    cout << "  --help        Display this help message and exit" << endl;
    cout << "  --timings     Print out timings for each compilation step" << endl;
    cout << "  --ir        Print out generated LLVM IR (if compiling)" << endl;
    cout << "  --silent      Suppress output (except syntax errors)" << endl;
    cout << "  --truesilent  Suppress all output, including syntax errors" << endl;
    cout << "Note that the use of --silent or --truesilent will override output flags such --timings." << endl;
}

int main(int argc, char* argv[]) {
    //Check for --help flag before anything else
    if (cmdOptionExists(argv, argv + argc, "--help")) {
        displayHelp();
        return 0;
    }

    //Check for the secret command
    if (argc > 1 && string(argv[1]) == "secret") {
        //Display a random Easter egg
        srand(time(nullptr));
        int choice = rand() % 5;

        switch (choice) {
            case 0:
                displayAsciiArt();
                break;
            case 1:
                displayHiddenMessage();
                break;
            case 2:
                playGuessingGame();
                break;
            case 3:
                displayRandomJoke();
                break;
            case 4:
                activateSecretFeature();
                break;
        }
        return 0;
    }

    //Requires at least 2 arguments: the command and the filepath
    if (argc < 3) {
        if (!cmdOptionExists(argv, argv + argc, "--truesilent")) {
            cerr << "Missing arguments." << endl;
            cerr << "For usage information: startasm --help" << endl;
        }
        return 1;
    }

    string command(argv[1]);
    if (command != "compile" && command!= "ast") {
        if (!cmdOptionExists(argv, argv + argc, "--truesilent")) {
            cerr << "Unknown command: " << command << endl;
            cerr << "For usage information: startasm --help" << endl;
        }
        return 1;
    }

    string filepath(argv[2]);
    if (!isValidSASMFile(filepath) && !cmdOptionExists(argv, argv + argc, "--truesilent")) {
        cerr << "Error: The file must have a .sasm extension." << endl;
        return 1;
    }

    //Flags
    bool timings = cmdOptionExists(argv, argv + argc, "--timings");
    bool ir = cmdOptionExists(argv, argv + argc, "--ir");
    bool silent = cmdOptionExists(argv, argv + argc, "--silent") || cmdOptionExists(argv, argv + argc, "--truesilent");
    bool truesilent = cmdOptionExists(argv, argv + argc, "--truesilent");

    //Adjust the compiler instantiation to pass the truesilent flag
    Compiler StartASMCompiler(filepath, silent, timings, ir);
    double start = omp_get_wtime();
    if (command == "compile") {
        if (!StartASMCompiler.compileCode()) {
            if (!truesilent) {
                cerr << StartASMCompiler.getStatus() << endl;
            }
        }
        else {
            double end = omp_get_wtime();

            if (timings && !silent) {
                cout << "Total time taken: " << (end - start) << " seconds\n";
            }

            if (!silent) {
                cout << StartASMCompiler.getNumLines() << " lines compiled.\n";
            }
        }
    }
    else if (command == "ast") {
        if (!StartASMCompiler.outputAST()) {
            if (!truesilent) {
                cerr << StartASMCompiler.getStatus() << endl;
            }
        }
        else {
            double end = omp_get_wtime();

            if (timings && !silent) {
                cout << "Total time taken: " << (end - start) << " seconds\n";
            }

            if (!silent) {
                cout << "Abstract Syntax Tree generated.\n";
            }
        }
    }
    return 0;
}