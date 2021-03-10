#include "assembler_src.hpp"
#include <fstream>
#include <iostream>

int main() {

    vector<string> instructionsBinary;

    // read the input file line by line
    ifstream inputFile(".text");
    string line;
    while (getline(inputFile, line)){

        // do something else...

        // when seeing `.text`
        vector<string> rawInstructions;
        if (line == ".text") {
            while (getline(inputFile, line))
                rawInstructions.push_back(line);

            instructionsBinary = startAssembler(rawInstructions);
        }
    }


    // now the `instructionsBinary` is ready

    // do other things
    // debug
    for (string machineCode : instructionsBinary){
        cout << machineCode << endl;
    }
}
