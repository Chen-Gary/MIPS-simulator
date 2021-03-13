/*
 * This file contains the function, `simulateToExecute`
 */

#include <bitset>
using namespace std;


void simulateToExecute(const uint32_t* PC, const map<string, uint32_t*> & str2SimulatedRegister){
    uint32_t instruction = *PC; // fetch instruction
    string instructionStr = bitset<32>(instruction).to_string();

    // R-type: op + rs + rt + rd + shamt + funct
    // I-type: op + rs + rt + imm
    // J-type: op + addr
    string op = instructionStr.substr(0, 6);        // op_str
    string rs_str = instructionStr.substr(6, 5);
    string rt_str = instructionStr.substr(11, 5);
    string rd_str = instructionStr.substr(16, 5);
    string shamt_str = instructionStr.substr(21, 5);
    string funct = instructionStr.substr(26, 6);    //funct_str
    string imm_str = instructionStr.substr(16, 16);
    string addr = instructionStr.substr(6, 26);     // addr_str

    // 会出错！！！
//    uint32_t* rs = str2SimulatedRegister.find(rs_str)->second;
//    uint32_t* rt = str2SimulatedRegister.find(rt_str)->second;
//    uint32_t* rd = str2SimulatedRegister.find(rd_str)->second;
//    // shamt_str --> shamt
//    // imm_str --> imm


    // check which instruction to execute
    // 1. add
    if (op == "000000" && funct == "100000")
        cout << "add!!" << endl;
    // 2. addu

    // 3. addi
    else if (op == "001000")
        cout << "addi!!" << endl;
    // 4. addiu

    // 5.and

    // 6. andi

    // 7. clo

    // 8. clz

    // 9. div

    // 10. divu

    // 11. mult

    // 12. multu

    // 13. mul

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
    else if (op == "000000" && funct == "001100")
        cout << "syscall!!" << endl;
    else {
        cout << "Unrecognized instruction in `simulateToExecute()`" << endl;
        throw;
    }
}
