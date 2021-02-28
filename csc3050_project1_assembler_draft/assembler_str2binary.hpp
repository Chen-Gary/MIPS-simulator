#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
using namespace std;

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

