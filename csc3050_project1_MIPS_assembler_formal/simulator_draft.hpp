/*
 * This file contains functions related to simulator.
 */

#include "simulator_simulateToExecute.hpp"
#include <bitset>
#include <vector>
#include <string>
using namespace std;

// tokenize input str, and store the tokens in a vector
void tokenizeLine_forDataSegment(const string & str, vector<string> * tokens) {
    size_t prev=0, pos;
    // delimiters: space, comma, tab
    while ((pos = str.find_first_of(" ,\t", prev)) != string::npos) {
        if (pos > prev)
            tokens->push_back(str.substr(prev, pos-prev));
        prev = pos + 1;
    }
    if (prev < str.length())
        tokens->push_back(str.substr(prev, string::npos));
}


char checkEscapeCharacter(size_t & i, char nextChar){
    i++;
    switch (nextChar) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '\\':
            return '\\';
        case '\'':
            return '\'';
        case '\"':
            return '\"';
        case '\?':
            return '\?';
        case '0':
            return '\0';
        default:         // it is not a escape character
            i--;
            return '\\';
    }
}

// The following functions are used in `placeStaticDataToMemory()`
// return updated `staticDataSegmentCurrent`
uint8_t* asciiz_toMemory(const string & line, uint8_t* addrCurrent){
    size_t indexOpenQuotationMark = line.find_first_of('"');
    size_t indexCloseQuotationMark = line.find_last_of('"');

    string valueStr = line.substr(indexOpenQuotationMark+1, indexCloseQuotationMark - indexOpenQuotationMark - 1);

    vector<char> valueChar;
    for (size_t i=0; i<valueStr.length(); i++){
        if (valueStr[i] == '\\' && i+1 < valueStr.length()){ // check escape character
            char nextChar = valueStr[i+1];
            valueChar.push_back( checkEscapeCharacter(i, nextChar) );
        } else {
            valueChar.push_back(valueStr[i]);
        }
    }
    valueChar.push_back('\0'); // since it is asciiz type
    // now `valueChar` is ready

    // place char in `valueChar` to memory
    char* addr = (char*) addrCurrent;
    for (char ch : valueChar){
        *addr = ch;
        addr++;
    }

    // valueChar.size() --> the actual bytes we used
    // we need to extend it to the next multiple of 4
    size_t offset = (valueChar.size() + 3) & ~0x03; // next multiple of 4
    addrCurrent = addrCurrent + offset;
    return addrCurrent;
}

// The only difference between `asciiz_toMemory` and `ascii_toMemory` is that
// there is no extra `valueChar.push_back('\0');` in `ascii_toMemory`
uint8_t* ascii_toMemory(const string & line, uint8_t* addrCurrent){
    size_t indexOpenQuotationMark = line.find_first_of('"');
    size_t indexCloseQuotationMark = line.find_last_of('"');

    string valueStr = line.substr(indexOpenQuotationMark+1, indexCloseQuotationMark - indexOpenQuotationMark - 1);

    vector<char> valueChar;
    for (size_t i=0; i<valueStr.length(); i++){
        if (valueStr[i] == '\\' && i+1 < valueStr.length()){ // check escape character
            char nextChar = valueStr[i+1];
            valueChar.push_back( checkEscapeCharacter(i, nextChar) );
        } else {
            valueChar.push_back(valueStr[i]);
        }
    }
    // now `valueChar` is ready

    // place char in `valueChar` to memory
    char* addr = (char*) addrCurrent;
    for (char ch : valueChar){
        *addr = ch;
        addr++;
    }

    // valueChar.size() --> the actual bytes we used
    // we need to extend it to the next multiple of 4
    size_t offset = (valueChar.size() + 3) & ~0x03; // next multiple of 4
    addrCurrent = addrCurrent + offset;
    return addrCurrent;
}

uint8_t* word_toMemory(const string & line, uint8_t* addrCurrent){
    size_t indexOfDot = line.find_first_of('.');
    string valueStr = line.substr(indexOfDot+5);

    vector<string> tokens;
    tokenizeLine_forDataSegment(valueStr, &tokens);

    vector<uint32_t> value;
    for (string token : tokens){
        uint32_t num = stoi(token, nullptr, 0);
        value.push_back(num);
    }
    // now `value` is ready

    // place uint32_t in `value` to memory
    uint32_t* addr = (uint32_t*) addrCurrent;
    for (uint32_t num : value){
        *addr = num;
        addr++;
    }

    size_t offset = value.size() * 4;  // value.size() --> # of words (# of 4-bytes)
    addrCurrent = addrCurrent + offset;

    return addrCurrent;
}


uint8_t* byte_toMemory(const string & line, uint8_t* addrCurrent){
    size_t indexOfDot = line.find_first_of('.');
    string valueStr = line.substr(indexOfDot+5);

    vector<string> tokens;
    tokenizeLine_forDataSegment(valueStr, &tokens);

    vector<uint8_t> value;
    for (string token : tokens){
        uint8_t num = stoi(token, nullptr, 0);
        value.push_back(num);
    }
    // now `value` is ready

    // place uint8_t in `value` to memory
    uint8_t* addr = addrCurrent;
    for (uint8_t num : value){
        *addr = num;
        addr++;
    }

    size_t offset = (value.size() + 3) & ~0x03; // value.size() --> # of bytes ==> find next multiple of 4
    addrCurrent = addrCurrent + offset;

    return addrCurrent;
}


uint8_t* half_toMemory(const string & line, uint8_t* addrCurrent){
    size_t indexOfDot = line.find_first_of('.');
    string valueStr = line.substr(indexOfDot+5);

    vector<string> tokens;
    tokenizeLine_forDataSegment(valueStr, &tokens);

    vector<uint16_t> value;
    for (string token : tokens){
        uint16_t num = stoi(token, nullptr, 0);
        value.push_back(num);
    }
    // now `value` is ready

    // place uint16_t in `value` to memory
    uint16_t* addr = (uint16_t*) addrCurrent;
    for (uint16_t num : value){
        *addr = num;
        addr++;
    }

    size_t offset = (value.size()*2 + 3) & ~0x03; // value.size() --> # of 2-bytes ==> find next multiple of 4
    addrCurrent = addrCurrent + offset;

    return addrCurrent;
}


// Put the data in .data segment of MIPS file piece by piece in the static data segment.
// The basic syntax of raw data is: <name>: .<data_type> <value>
// e.g. str1: .asciiz "hello world!\n"
// Since `<name>:` is not used in this simulator, we simply ignore this part
// The data type supported: `ascii`, `asciiz`, `word`, `byte`, `half`
// return: the end of static data section (uint8_t*)
uint8_t* placeStaticDataToMemory(const vector<string> & rawDataSegment, uint8_t * staticDataSegmentStart){
    uint8_t * staticDataSegmentCurrent = staticDataSegmentStart;

    // go through every line of raw data
    for (string line : rawDataSegment){

        // find and remove comment in the line
        size_t posComment = line.find_first_of('#');
        if (posComment != string::npos) {               // comment found
            line = line.substr(0, posComment);     // remove the comment
        }

        // identify which data type it is
        size_t indexOfDot = line.find_first_of('.');

        // if we do not find `.`, then we assume this line is empty
        if (indexOfDot == string::npos)
            continue;

        if (line[indexOfDot+1] == 'a') {                // `ascii` or `asciiz`
            if (line.substr(indexOfDot+1, 6) == "asciiz") { // `asciiz` (2)
                staticDataSegmentCurrent = asciiz_toMemory(line, staticDataSegmentCurrent);
            } else if (line.substr(indexOfDot+1, 5) == "ascii") { // `ascii` (1)
                staticDataSegmentCurrent = ascii_toMemory(line, staticDataSegmentCurrent);
            } else {
                cout << "Unrecognized data type in `placeStaticDataToMemory` --- `.a`" << endl;
                throw;
            }
        }
        else if (line[indexOfDot+1] == 'w') {           // `word` (3)
            staticDataSegmentCurrent = word_toMemory(line, staticDataSegmentCurrent);
        }
        else if (line[indexOfDot+1] == 'b') {           // `byte` (4)
            staticDataSegmentCurrent = byte_toMemory(line, staticDataSegmentCurrent);
        }
        else if (line[indexOfDot+1] == 'h') {           // `half` (5)
            staticDataSegmentCurrent = half_toMemory(line, staticDataSegmentCurrent);
        } else {
            cout << "Unrecognized data type in `placeStaticDataToMemory`" << endl;
            throw;
        }
    }


//    // debug
//    printf("%x\n", staticDataSegmentStart);
//    printf("%x\n", staticDataSegmentCurrent);
//    //debug
//    cout << "debug: half_toMemory" << endl;
//    uint16_t * addr = (uint16_t*) staticDataSegmentStart;
//    for (int i=0; i<3; i++){
//        cout << *addr << endl;
//        addr++;
//    }


    return staticDataSegmentCurrent;
}


void startSimulator(const vector<string> & instructionsBinary, const vector<string> & rawDataSegment){

    // Dynamically allocate 6MB to simulate memory (RAM)
    uint8_t * memoryStart = new uint8_t [6 * 1024 * 1024];          // fake address 0x400000
    uint32_t * textSegmentStart = (uint32_t*) memoryStart;          // fake address 0x400000 (decimal 4194304)
    uint8_t * staticDataSegmentStart = memoryStart + 1024 * 1024;   // fake address 0x500000 (decimal 5242880)
    uint8_t * dynamicDataSegmentStart;                              // wherever static data section ends
    uint8_t * stackSegmentStart = memoryStart + 6 * 1024 * 1024;    // fake address 0xa00000

    // Simulate registers (the address store in these fake registers are all fake address, instead of real address)
    uint32_t * zero_reg = new uint32_t {0};
    uint32_t * at_reg = new uint32_t {0};
    uint32_t * v0_reg = new uint32_t {0};
    uint32_t * v1_reg = new uint32_t {0};
    uint32_t * a0_reg = new uint32_t {0};
    uint32_t * a1_reg = new uint32_t {0};
    uint32_t * a2_reg = new uint32_t {0};
    uint32_t * a3_reg = new uint32_t {0};
    uint32_t * t0_reg = new uint32_t {0};
    uint32_t * t1_reg = new uint32_t {0};
    uint32_t * t2_reg = new uint32_t {0};
    uint32_t * t3_reg = new uint32_t {0};
    uint32_t * t4_reg = new uint32_t {0};
    uint32_t * t5_reg = new uint32_t {0};
    uint32_t * t6_reg = new uint32_t {0};
    uint32_t * t7_reg = new uint32_t {0};
    uint32_t * s0_reg = new uint32_t {0};
    uint32_t * s1_reg = new uint32_t {0};
    uint32_t * s2_reg = new uint32_t {0};
    uint32_t * s3_reg = new uint32_t {0};
    uint32_t * s4_reg = new uint32_t {0};
    uint32_t * s5_reg = new uint32_t {0};
    uint32_t * s6_reg = new uint32_t {0};
    uint32_t * s7_reg = new uint32_t {0};
    uint32_t * t8_reg = new uint32_t {0};
    uint32_t * t9_reg = new uint32_t {0};
    uint32_t * k0_reg = new uint32_t {0};
    uint32_t * k1_reg = new uint32_t {0};
    uint32_t * gp_reg = new uint32_t {0};//???
    uint32_t * sp_reg = new uint32_t {10485760};// 10485760=0xa00000; "For example, the stack pointer register, $sp, should always store the current stack top. You should initialize it with a value of 1000000_hex."
    uint32_t * fp_reg = new uint32_t {0};//???
    uint32_t * ra_reg = new uint32_t {0};

    // map from binary_string_addr to register (which is uint32_t*)
    const map<string, uint32_t*> str2SimulatedRegister = {
            {"00000", zero_reg},
            {"00001", at_reg},
            {"00010", v0_reg},
            {"00011", v1_reg},
            {"00100", a0_reg},
            {"00101", a1_reg},
            {"00110", a2_reg},
            {"00111", a3_reg},
            {"01000", t0_reg},
            {"01001", t1_reg},
            {"01010", t2_reg},
            {"01011", t3_reg},
            {"01100", t4_reg},
            {"01101", t5_reg},
            {"01110", t6_reg},
            {"01111", t7_reg},
            {"10000", s0_reg},
            {"10001", s1_reg},
            {"10010", s2_reg},
            {"10011", s3_reg},
            {"10100", s4_reg},
            {"10101", s5_reg},
            {"10110", s6_reg},
            {"10111", s7_reg},
            {"11000", t8_reg},
            {"11001", t9_reg},
            {"11010", k0_reg},
            {"11011", k1_reg},
            {"11100", gp_reg},
            {"11101", sp_reg},
            {"11110", fp_reg},
            {"11111", ra_reg},
    };


    // Put the data in .data segment of MIPS file piece by piece in the static data segment.
    dynamicDataSegmentStart = placeStaticDataToMemory(rawDataSegment, staticDataSegmentStart);


    // Put the assembled machine code in the .text segment of the simulated memory
    // Note that the machine code is store as little endian in this method
    uint32_t * textSegmentCurrent = textSegmentStart;
    for (string machineCodeStr : instructionsBinary){
        *textSegmentCurrent = bitset<32>(machineCodeStr).to_ulong();
        textSegmentCurrent++;
    }


    // Start simulating
    uint32_t * PC_realAddr = textSegmentStart;
    int protection = 0;
    const int PROTECT_RANGE = 11;
    while (protection < PROTECT_RANGE){
    //while (true){
        simulateToExecute(PC_realAddr, str2SimulatedRegister);

        // PC = PC + 4 is calculated inside `simulateToExecute()`
        protection++;
    }
    // debug
    if (protection >= PROTECT_RANGE)
        cout << "protection exceed!!!" << endl;



    // simulation ends
    // free memory
    delete [] memoryStart;
    delete zero_reg;
    delete at_reg;
    delete v0_reg;
    delete v1_reg;
    delete a0_reg;
    delete a1_reg;
    delete a2_reg;
    delete a3_reg;
    delete t0_reg;
    delete t1_reg;
    delete t2_reg;
    delete t3_reg;
    delete t4_reg;
    delete t5_reg;
    delete t6_reg;
    delete t7_reg;
    delete s0_reg;
    delete s1_reg;
    delete s2_reg;
    delete s3_reg;
    delete s4_reg;
    delete s5_reg;
    delete s6_reg;
    delete s7_reg;
    delete t8_reg;
    delete t9_reg;
    delete k0_reg;
    delete k1_reg;
    delete gp_reg;
    delete sp_reg;
    delete fp_reg;
    delete ra_reg;
}
