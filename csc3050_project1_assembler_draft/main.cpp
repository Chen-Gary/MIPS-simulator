/*
 * overall implementation of assembler:
 * 1. scan the .text file line by line
 *      - when encountering instruction, store the whole instruction as a string in a vector (tokenize the instruction!)
 *      - when encountering label, store the label and corresponding address in a map
 *      - when encountering comment, simply ignore the comment
 *
 * 2. translate the instructions to 32-bit binary strings
 */

#include "assembler_str2binary.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


// tokenize input str, and store the tokens in a vector
void tokenizeLine(const string & str, vector<string> * tokens) {
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


// input a tokenized line of instruction
// return the corresponding binary code
string str2binary(vector<string>* line) {
    string result;

    string indicator = (*line)[0];
    // 1. add
    if (indicator == "add")
        result = add_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 2. addu
    else if (indicator == "addu")
        result = addu_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 3. addi
    else if (indicator == "addi")
        result = addi_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 4. addiu
    else if (indicator == "addiu")
        result = addiu_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 5.and
    else if (indicator == "and")
        result = and_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 6. andi
    else if (indicator == "andi")
        result = andi_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 7. clo
    else if (indicator == "clo")
        result = clo_str2binary((*line)[1], (*line)[2]);
    // 8. clz
    else if (indicator == "clz")
        result = clz_str2binary((*line)[1], (*line)[2]);
    // 9. div
    else if (indicator == "div")
        result = div_str2binary((*line)[1], (*line)[2]);
    // 10. divu
    else if (indicator == "divu")
        result = divu_str2binary((*line)[1], (*line)[2]);
    // 11. mult
    else if (indicator == "mult")
        result = mult_str2binary((*line)[1], (*line)[2]);
    // 12. multu
    else if (indicator == "multu")
        result = multu_str2binary((*line)[1], (*line)[2]);
    // 13. mul
    else if (indicator == "mul")
        result = mul_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 14. madd

    // 15. msub

    // 16. maddu

    // 17.msubu

    // 18.nor

    // 19.or

    // 20.ori

    // 21.sll

    // 22.sllv

    // 23.sra

    // 24.srav

    // 25.srl

    // 26.srlv

    // 27.sub

    // 28.subu

    // 29.xor

    // 30.xori

    // 31.lui

    // 32.slt

    // 33.sltu

    // 34.slti

    // 35.sltiu

    // 36.beq

    // 37.bgez

    // 38.bgezal

    // 39.bgtz

    // 40.blez

    // 41.bltzal

    // 42.bltz

    // 43.bne

    // 44.j

    // 45.jal

    // 46.jalr

    // 47.jr

    // 48.teq

    // 49.teqi

    // 50.tne

    // 51.tnei

    // 52.tge

    // 53.tgeu

    // 54.tgei

    // 55.tgeiu

    // 56.tlt

    // 57.tltu

    // 58.tlti

    // 59.tltiu

    // 60.lb

    // 61.lbu

    // 62.lh

    // 63.lhu

    // 64.lw

    // 65.lwl

    // 66.lwr

    // 67.ll

    // 68.sb

    // 69.sh

    // 70.sw

    // 71.swl

    // 72.swr

    // 73.sc

    // 74.mfhi

    // 75.mflo

    // 76.mthi

    // 77.mtlo

    // 78.syscall


    // debug
    cout << result << endl;

    return result;
}


int main() {

    vector< vector<string>* > instructions;      // store all lines of instructions, each as a nested vector (pointer)
    int lineNum = -1;                            // when a line is pushed to `instructions`, `lineNum` increases by one
    map<string, int> label2offset;               // pair(key=label, value=offset)
    vector<string> instructionsBinary;

    // read the input file line by line
    ifstream inputFile(".text");
    string line;
    while (getline(inputFile, line)) {

        // find and remove comment in the line
        size_t posComment = line.find_first_of('#');
        if (posComment != string::npos) {               // comment found
            line = line.substr(0, posComment);     // remove the comment
        }

        // find and remove label in the line
        size_t posLabel = line.find_first_of(':');
        if (posLabel != string::npos) {
            string label = line.substr(0, posLabel);
            // remove leading/ending spaces or tabs of the label
            vector<string> * labels = new vector<string>;
            tokenizeLine(label, labels);
            if (labels->size() == 1) {
                label = (*labels)[0];
                delete labels;
            } else {
                cout << "Exception: invalid label name!" << endl;
                throw;
            }
            label2offset.insert(pair<string, int>(label, lineNum+1));
            line = line.substr(posLabel+1);
        }

        // tokenize the line
        vector<string> * tokens = new vector<string>;
        tokenizeLine(line, tokens);
        if (!tokens->empty()) {
            instructions.push_back(tokens);
            lineNum++;
        } else { // if the vector* is empty
            delete tokens;
        }
    }

    // translate instructions to 32-bit binary strings line by line
    for (vector<string>* line : instructions) {
        instructionsBinary.push_back( str2binary(line) );
    }



//    // debug
//    for (vector<string>* tokens : instructions){
//        cout << "Line: " << endl;
//        for (string token : *tokens) {
//            cout << token << "  |  ";
//        }
//        cout << endl;
//    }
//    cout << "\noutput map:" << endl;
//    for (auto i : label2offset) {
//        cout << i.first << " : " << i.second << endl;
//    }


    // free memory
    for (vector<string>* tokens : instructions){
        delete tokens;
    }


    return 0;
}
