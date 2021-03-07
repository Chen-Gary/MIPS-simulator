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
string str2binary(vector<string>* line, int currentLineNum, const map<string, int> & label2offset) {
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
    else if (indicator == "madd")
        result = madd_str2binary((*line)[1], (*line)[2]);
    // 15. msub
    else if (indicator == "msub")
        result = msub_str2binary((*line)[1], (*line)[2]);
    // 16. maddu
    else if (indicator == "maddu")
        result = maddu_str2binary((*line)[1], (*line)[2]);
    // 17.msubu
    else if (indicator == "msubu")
        result = msubu_str2binary((*line)[1], (*line)[2]);
    // 18.nor
    else if (indicator == "nor")
        result = nor_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 19.or
    else if (indicator == "or")
        result = or_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 20.ori
    else if (indicator == "ori")
        result = ori_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 21.sll
    else if (indicator == "sll")
        result = sll_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 22.sllv
    else if (indicator == "sllv")
        result = sllv_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 23.sra
    else if (indicator == "sra")
        result = sra_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 24.srav
    else if (indicator == "srav")
        result = srav_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 25.srl
    else if (indicator == "srl")
        result = srl_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 26.srlv
    else if (indicator == "srlv")
        result = srlv_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 27.sub
    else if (indicator == "sub")
        result = sub_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 28.subu
    else if (indicator == "subu")
        result = subu_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 29.xor
    else if (indicator == "xor")
        result = xor_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 30.xori
    else if (indicator == "xori")
        result = xori_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 31.lui
    else if (indicator == "lui")
        result = lui_str2binary((*line)[1], (*line)[2]);
    // 32.slt
    else if (indicator == "slt")
        result = slt_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 33.sltu
    else if (indicator == "sltu")
        result = sltu_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 34.slti
    else if (indicator == "slti")
        result = slti_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 35.sltiu
    else if (indicator == "sltiu")
        result = sltiu_str2binary((*line)[1], (*line)[2], (*line)[3]);
    // 36.beq
    else if (indicator == "beq")
        result = beq_str2binary((*line)[1], (*line)[2], (*line)[3], currentLineNum, label2offset);
    // 37.bgez
    else if (indicator == "bgez")
        result = bgez_str2binary((*line)[1], (*line)[2], currentLineNum, label2offset);
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


    return result;
}

// assembler: when seeing `.text`, this program starts to translate mips instruction to binary strings
int main() {

    vector< vector<string>* > instructions;      // store all lines of instructions, each as a nested vector (pointer)
    int lineNum = -1;                            // when a line is pushed to `instructions`, `lineNum` increases by one
    map<string, int> label2offset;               // pair(key=label, value=offset)
    vector<string> instructionsBinary;

    // read the input file line by line
    ifstream inputFile(".text");
    string line;
    while (getline(inputFile, line)) {
        // if the first line of instruction is `.text`, simply ignore it
        if (line == ".text") {
            //cout << "remove `.text`" << endl; //debug
            continue;
        }

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
    for (int lineIndex=0; lineIndex<instructions.size(); lineIndex++){
        // `lineIndex` is the number of current line of instruction
        // ranging from 0, 1, 2,..., (instructions.size()-1)
        // with the info of `label2offset`, the offset can be calculated
        instructionsBinary.push_back( str2binary( instructions[lineIndex], lineIndex, label2offset ) );
    }
//    for (vector<string>* line : instructions) {
//        instructionsBinary.push_back( str2binary(line) );
//    }


    // debug
    for (string machineCode : instructionsBinary){
        cout << "In `instructionsBinary`: " << machineCode << endl;
    }
    cout << "check the label map: " << endl;
    for (auto i : label2offset) {
        cout << i.first << " : " << i.second << endl;
    }


//    // debug
//    for (vector<string>* tokens : instructions){
//        cout << "Line: " << endl;
//        for (string token : *tokens) {
//            cout << token << "  |  ";
//        }
//        cout << endl;
//    }



    // free memory
    for (vector<string>* tokens : instructions){
        delete tokens;
    }


    return 0;
}
