#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
using namespace std;

// global var!!!
const map<string, string> registerMap = {
        {"$zero", "00000"}, // 0
        {"$at", "00001"},    // 1
        {"$v0", "00010"},    // 2
        {"$v1", "00011"},    // 3
        {"$a0", "00100"},    // 4
        {"$a1", "00101"},    // 5
        {"$a2", "00110"},    // 6
        {"$a3", "00111"},    // 7
        {"$t0", "01000"},    // 8
        {"$t1", "01001"},    // 9
        {"$t2", "01010"},    // 10
        {"$t3", "01011"},    // 11
        {"$t4", "01100"},    // 12
        {"$t5", "01101"},    // 13
        {"$t6", "01110"},    // 14
        {"$t7", "01111"},    // 15
        {"$s0", "10000"},    // 16
        {"$s1", "10001"},    // 17
        {"$s2", "10010"},    // 18
        {"$s3", "10011"},    // 19
        {"$s4", "10100"},    // 20
        {"$s5", "10101"},    // 21
        {"$s6", "10110"},    // 22
        {"$s7", "10111"},    // 23
        {"$t8", "11000"},    // 24
        {"$t9", "11001"},    // 25
        {"$k0", "11010"},    // 26
        {"$k1", "11011"},    // 27
        {"$gp", "11100"},    // 28
        {"$sp", "11101"},    // 29
        {"$fp", "11110"},    // 30
        {"$ra", "11111"},    // 31
};


string intTo16BitBinarySigned(const string & str) {
    int decimal = stoi(str);
    // int to 16-bit signed binary string
    string result = bitset<16>(decimal).to_string();
    return result;
}

string intTo16BitBinaryUnsigned(const string & str){
    unsigned decimal = stoul(str);
    // unsigned to 16-bit unsigned binary string
    string result = bitset<16>(decimal).to_string();
    return result;
}

string shamtStrToBinary(const string & str){ // I am not sure how to deal with shamt
    int decimal = stoi(str);
    // int to 5-bit binary string
    string result = bitset<5>(decimal).to_string();
    return result;
}

// I am not sure about the calculation of `offset`
string label2offset16bit(const string & label, int currentLineNum, const map<string, int> & label2offset){
    // get the lineNum of target line of instruction
    // ranging from {0, 1, 2,...}
    int targetLineNum = label2offset.find(label)->second;
    // PC = PC + 4 + offset*4
    // offset = targetLineNum - currentLineNum - 1
    int difference = targetLineNum - currentLineNum - 1;
    string offset = bitset<16>(difference).to_string();
    return offset;
}

// for j-instructions
// I am not sure about the calculation of `target`
string label2offset26bit(const string & target, const map<string, int> & label2offset){
    int targetLineNum = label2offset.find(target)->second;
    // realTargetAddress = 0x400000 + targetLineNum * 4
    uint32_t realTargetAddress = 4194304 + targetLineNum * 4;
    string realTargetAddressStr = bitset<32>(realTargetAddress).to_string();
    // realTargetAddressStr = "PC first 4" + "target26" + "00"
    string result = realTargetAddressStr.substr(4, 26);
    return result;
}

// Used in load/store related instructions
// e.g. lw $t0, 32($s3)
// input address = 32($s3) ---> output rs=$s3, offset=32
void furtherTokenizeForRsAndOffset(const string & address, string & rs, string & offset){
    size_t indexOpenParenthesis = address.find_first_of('(');
    size_t indexCloseParenthesis = address.find_first_of(')');

    if (indexOpenParenthesis == string::npos || indexCloseParenthesis == string::npos){
        cout << "`furtherTokenizeForRsAndOffset` error: no '(' or ')' found in the address!!" << endl;
        throw;
    }

    // output rs=$s3, offset=32
    rs = address.substr(indexOpenParenthesis+1, indexCloseParenthesis-indexOpenParenthesis-1);
    offset = address.substr(0, indexOpenParenthesis);
    // handle cases, e.g. sb $a1, ($a0)
    if (offset.empty())
        offset = "0";
}

// 1. add
string add_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100000";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 2. addu
string addu_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100001";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 3. addi
string addi_str2binary(string rt, string rs, string imm){
    string op = "001000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}

// 4. addiu
string addiu_str2binary(string rt, string rs, string imm){
    string op = "001001";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinaryUnsigned(imm);
    string result = op + rs + rt + imm;
    return result;
}

// 5. and
string and_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100100";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 6. andi
string andi_str2binary(string rt, string rs, string imm){
    string op = "001100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}

// 7. clo
string clo_str2binary(string rd, string rs){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    string rt = "00000";
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100001";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 8. clz
string clz_str2binary(string rd, string rs){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    string rt = "00000";
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100000";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 9. div
string div_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "011010";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 10. divu
string divu_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "011011";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 11. mult
string mult_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "011000";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 12. multu
string multu_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "011001";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 13. mul
string mul_str2binary(string rd, string rs, string rt){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "000010";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 14. madd
string madd_str2binary(string rs, string rt){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "000000";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 15. msub
string msub_str2binary(string rs, string rt){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "000100";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 16. maddu
string maddu_str2binary(string rs, string rt){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "000001";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 17.msubu
// typo in the textbook????
string msubu_str2binary(string rs, string rt){
    string op = "011100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "000101";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}

// 18.nor
string nor_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100111";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 19.or
string or_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100101";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 20.ori
string ori_str2binary(string rt, string rs, string imm){
    string op = "001101";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinaryUnsigned(imm); // zero-extended imm
    string result = op + rs + rt + imm;
    return result;
}

// 21.sll
// typo in the textbook????
string sll_str2binary(string rd, string rt, string shamt){
    string op = "000000";
    string rs = "00000";
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    shamt = shamtStrToBinary(shamt);
    string funct = "000000";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 22.sllv
// typo in the textbook????
// sllv rd, rt, rs (in the textbook, and I follow this)
// the order of (rd, rs, rt) is more usual
string sllv_str2binary(string rd, string rt, string rs){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "000100";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 23.sra
string sra_str2binary(string rd, string rt, string shamt){
    string op = "000000";
    string rs = "00000";    // typo in the textbook????
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    shamt = shamtStrToBinary(shamt);
    string funct = "000011";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 24.srav
string srav_str2binary(string rd, string rt, string rs){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "000111";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 25.srl
string srl_str2binary(string rd, string rt, string shamt){
    string op = "000000";
    string rs = "00000";
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    shamt = shamtStrToBinary(shamt);
    string funct = "000010";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 26.srlv
string srlv_str2binary(string rd, string rt, string rs){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "000110";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 27.sub
string sub_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100010";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 28.subu
string subu_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100011";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 29.xor
string xor_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "100110";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 30.xori
string xori_str2binary(string rt, string rs, string imm){
    string op = "001110";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinaryUnsigned(imm); // zero-extended imm
    string result = op + rs + rt + imm;
    return result;
}

// 31.lui
string lui_str2binary(string rt, string imm){
    string op = "001111";
    string rs = "00000";
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinaryUnsigned(imm);
    string result = op + rs + rt + imm;
    return result;
}

// 32.slt
string slt_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "101010";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 33.sltu
string sltu_str2binary(string rd, string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "101011";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}

// 34.slti
string slti_str2binary(string rt, string rs, string imm){
    string op = "001010";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}

// 35.sltiu
string sltiu_str2binary(string rt, string rs, string imm){
    string op = "001011";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}

// 36.beq
string beq_str2binary(string rs, string rt, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000100";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 37.bgez
string bgez_str2binary(string rs, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "00001";
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 38.bgezal
string bgezal_str2binary(string rs, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "10001";
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 39.bgtz
string bgtz_str2binary(string rs, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000111";
    rs = registerMap.find(rs)->second;
    string rt = "00000";
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 40.blez
string blez_str2binary(string rs, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000110";
    rs = registerMap.find(rs)->second;
    string rt = "00000";
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 41.bltzal
string bltzal_str2binary(string rs, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "10000";
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 42.bltz
string bltz_str2binary(string rs, const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "00000";
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 43.bne
string bne_str2binary(string rs, string rt,const string & label, int currentLineNum, const map<string, int> & label2offset){
    string op = "000101";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string offset = label2offset16bit(label, currentLineNum, label2offset);
    string result = op + rs + rt + offset;
    return result;
}
// 44.j
string j_str2binary(string target, const map<string, int> & label2offset){
    string op = "000010";
    target = label2offset26bit(target, label2offset);
    string result = op + target;
    return result;
}
// 45.jal
string jal_str2binary(string target, const map<string, int> & label2offset){
    string op = "000011";
    target = label2offset26bit(target, label2offset);
    string result = op + target;
    return result;
}
// 46.jalr
string jalr_str2binary(string rs, string rd){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    string rt = "00000";
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "001001";
    string result = op + rs + rt + rd + shamt + funct;
    return result;
}
// 47.jr
string jr_str2binary(string rs){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    string rtRdShamt = "000000000000000";
    string funct = "001000";
    string result = op + rs + rtRdShamt + funct;
    return result;
}
// 48.teq
string teq_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "110100";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}
// 49.teqi
string teqi_str2binary(string rs, string imm){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "01100";
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}
// 50.tne
// typo in the textbook????
string tne_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "110110";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}
// 51.tnei
// typo in the textbook????
string tnei_str2binary(string rs, string imm){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "01110";
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}
// 52.tge
string tge_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "110000";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}
// 53.tgeu
string tgeu_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "110001";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}
// 54.tgei
string tgei_str2binary(string rs, string imm){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "01000";
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}
// 55.tgeiu
string tgeiu_str2binary(string rs, string imm){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "01001";
    imm = intTo16BitBinarySigned(imm); //sign-extended value imm, I am not sure
    string result = op + rs + rt + imm;
    return result;
}
// 56.tlt
string tlt_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "110010";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}
// 57.tltu
string tltu_str2binary(string rs, string rt){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    string rdShamt = "0000000000";
    string funct = "110011";
    string result = op + rs + rt + rdShamt + funct;
    return result;
}
// 58.tlti
string tlti_str2binary(string rs, string imm){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "01010";
    imm = intTo16BitBinarySigned(imm);
    string result = op + rs + rt + imm;
    return result;
}
// 59.tltiu
string tltiu_str2binary(string rs, string imm){
    string op = "000001";
    rs = registerMap.find(rs)->second;
    string rt = "01011";
    imm = intTo16BitBinarySigned(imm); //sign-extended value imm, I am not sure
    string result = op + rs + rt + imm;
    return result;
}

// Note that the unit of `offset` is byte!!
// e.g. A[8] --> lw $t0, 32($s3)
// Here 32 = 4 * 8

// 60.lb
string lb_str2binary(string rt, const string & address){
    string op = "100000";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 61.lbu
string lbu_str2binary(string rt, const string & address){
    string op = "100100";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 62.lh
string lh_str2binary(string rt, const string & address){
    string op = "100001";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 63.lhu
string lhu_str2binary(string rt, const string & address){
    string op = "100101";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 64.lw
string lw_str2binary(string rt, const string & address){
    string op = "100011";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 65.lwl
string lwl_str2binary(string rt, const string & address){
    string op = "100010";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 66.lwr
string lwr_str2binary(string rt, const string & address){
    string op = "100110";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 67.ll
string ll_str2binary(string rt, const string & address){
    string op = "110000";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 68.sb
string sb_str2binary(string rt, const string & address){
    string op = "101000";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 69.sh
string sh_str2binary(string rt, const string & address){
    string op = "101001";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 70.sw
string sw_str2binary(string rt, const string & address){
    string op = "101011";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 71.swl
string swl_str2binary(string rt, const string & address){
    string op = "101010";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 72.swr
string swr_str2binary(string rt, const string & address){
    string op = "101110";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 73.sc
string sc_str2binary(string rt, const string & address){
    string op = "111000";
    string rs;
    rt = registerMap.find(rt)->second;
    string offset;

    furtherTokenizeForRsAndOffset(address, rs, offset);
    rs = registerMap.find(rs)->second;
    offset = intTo16BitBinarySigned(offset);

    string result = op + rs + rt + offset;
    return result;
}
// 74.mfhi
string mfhi_str2binary(string rd){
    string op = "000000";
    string rsRt = "0000000000";
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "010000";
    string result = op + rsRt + rd + shamt + funct;
    return result;
}
// 75.mflo
string mflo_str2binary(string rd){
    string op = "000000";
    string rsRt = "0000000000";
    rd = registerMap.find(rd)->second;
    string shamt = "00000";
    string funct = "010010";
    string result = op + rsRt + rd + shamt + funct;
    return result;
}
// 76.mthi
string mthi_str2binary(string rs){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    string rtRdShamt = "000000000000000";
    string funct = "010001";
    string result = op + rs + rtRdShamt + funct;
    return result;
}
// 77.mtlo
string mtlo_str2binary(string rs){
    string op = "000000";
    rs = registerMap.find(rs)->second;
    string rtRdShamt = "000000000000000";
    string funct = "010011";
    string result = op + rs + rtRdShamt + funct;
    return result;
}
// 78.syscall
string syscall_str2binary(){
    string op = "000000";
    string rsRtRdShamt = "00000000000000000000";
    string funct = "001100";
    string result = op + rsRtRdShamt + funct;
    return result;
}
