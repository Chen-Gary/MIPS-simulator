#include "assembler_src.hpp"
#include "simulator_start.hpp"
#include <fstream>
#include <iostream>

int main() {

    // read the input file line by line (into a vector)
    cout << "Please input the path of .asm file: ";
    string filePath;
    getline(cin, filePath);
    ifstream inputFile;
    inputFile.open(filePath);
    cout << "Program starts..." << endl;

    // separate the file into two segment, .data and .text
    bool isInDataSegment = false;
    bool isInTextSegment = false;
    vector<string> rawDataSegment;
    vector<string> rawTextSegment;
    string line;
    while (getline(inputFile, line)){
        if (isDotData(line)){ // the current line is `.data`
            isInDataSegment = true;
            isInTextSegment = false;
            continue;
        }
        if (isDotText(line)){ // the current line is `.text`
            isInDataSegment = false;
            isInTextSegment = true;
            continue;
        }

        if (isInDataSegment)
            rawDataSegment.push_back(line);

        if (isInTextSegment){
            rawTextSegment.push_back(line);
        }
    }
    // now `rawDataSegment` and `rawTextSegment` are ready


    // start assembler
    vector<string> instructionsBinary;
    instructionsBinary = startAssembler(rawTextSegment);
    // now the `instructionsBinary` is ready


//    //debug
//    cout << "Assembler result: " << endl;
//    for (const string & machineCode : instructionsBinary)
//        cout << machineCode << endl;


    // start simulator
    startSimulator(instructionsBinary, rawDataSegment);


//    // debug
//    cout << "rawDataSegment: " << endl;
//    for (const string & oneline : rawDataSegment)
//        cout << oneline << endl;
//    cout << "rawTextSegment: " << endl;
//    for (const string & oneline : rawTextSegment)
//        cout << oneline << endl;
}
