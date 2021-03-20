#include "assembler_src.hpp"
#include "simulator_start.hpp"
#include <iostream>


int main(int argc, char** argv) {

    // Three arguments should be included to execute this program (asm file, in file, out file)
    // sample command: ./simulator input_mips.asm input_mips.in input_mips.out
    if (argc < 4){
        cout << "Wrong number of arguments" << endl;
    }
    //cout << "Program starts..." << endl;

    ifstream asmFile;
    asmFile.open( argv[1] );
    inFile.open( argv[2] );
    outFile.open( argv[3] );
    //freopen(argv[2], "r", stdin);
    freopen(argv[3], "w", stdout); // redirect output (Note that here stdin is not redirected)

    // read the input asm file line by line (into a vector)
    // separate the file into two segment, .data and .text
    bool isInDataSegment = false;
    bool isInTextSegment = false;
    vector<string> rawDataSegment;
    vector<string> rawTextSegment;
    string line;
    while (getline(asmFile, line)){
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
    asmFile.close();
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
