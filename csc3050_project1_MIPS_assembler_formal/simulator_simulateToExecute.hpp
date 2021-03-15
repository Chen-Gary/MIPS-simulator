/*
 * This file contains the function, `simulateToExecute`
 */

#include "simulator_instruction_functions.hpp"
#include <bitset>
using namespace std;


int32_t signExtendedImm(uint32_t instruction){
    int32_t instructionSigned = (int32_t) instruction;
    int32_t imm_signExtended = (instructionSigned << 16) >>16;
    return imm_signExtended;
}

uint32_t zeroExtendedImm(uint32_t instruction){
    uint32_t imm_zeroExtended = (instruction << 16) >> 16;
    return imm_zeroExtended;
}

// Note that in this implementation we do not check which type (R, I or J) the current instruction is.
// Instead we directly use `op` and `funct` to identify which instruction PC_realAddr currently points to
void simulateToExecute(uint32_t* &PC_realAddr, const map<string, uint32_t*> & str2SimulatedRegister){
    uint32_t instruction = *PC_realAddr; // fetch instruction
    PC_realAddr++; // PC = PC + 4

    string instructionStr = bitset<32>(instruction).to_string();

    // R-type: op + rs + rt + rd + shamt + funct
    // I-type: op + rs + rt + imm
    // J-type: op + addr

    // `op` and `funct` identify which instruction PC_realAddr currently points to
    string op = instructionStr.substr(0, 6);        // op_str
    string funct = instructionStr.substr(26, 6);    //funct_str
    // Note that for `bgez`, `bgezal`..., rt_str is also used to identify instructions

    // get `rs`, `rt`, `rd`
    // although it is nonsense in some cases, e.g. there is no `rd` in I-type instruction
    string rs_str = instructionStr.substr(6, 5);
    string rt_str = instructionStr.substr(11, 5);
    string rd_str = instructionStr.substr(16, 5);
    uint32_t* rs = str2SimulatedRegister.find(rs_str)->second;
    uint32_t* rt = str2SimulatedRegister.find(rt_str)->second;
    uint32_t* rd = str2SimulatedRegister.find(rd_str)->second;

    // get `imm` (both sign-extended and zero-extended)
    // again it might be nonsense to do this for some instructions
    int32_t imm_signExtended = signExtendedImm(instruction);
    uint32_t imm_zeroExtended = zeroExtendedImm(instruction);

    // get `v0_reg` (do this for `syscall` instruction)
    uint32_t* v0_reg = str2SimulatedRegister.find("00010")->second;
    uint32_t* a0_reg = str2SimulatedRegister.find("00100")->second;

    // useless codes here
    //string shamt_str = instructionStr.substr(21, 5);
    //string imm_str = instructionStr.substr(16, 16);
    //string addr = instructionStr.substr(6, 26);     // addr_str



    // check which instruction to execute
    // 1. add
    if (op == "000000" && funct == "100000")
        add_toExecute(rs, rt, rd);
    // 2. addu
    else if (op == "000000" && funct == "100001")
        addu_toExecute(rs, rt, rd);
    // 3. addi
    else if (op == "001000")
        addi_toExecute(rs, rt, imm_signExtended);
    // 4. addiu
    else if (op == "001001")
        addiu_toExecute(rs, rt, imm_signExtended);
    // 5.and
    else if (op == "000000" && funct == "100100")
        printf("hello");
    // 6. andi
    else if (op == "001100")
        printf("");
    // 7. clo
    else if (op == "011100" && funct == "100001")
        printf("");
    // 8. clz
    else if (op == "011100" && funct == "100000")
        printf("");
    // 9. div
    else if (op == "000000" && funct == "011010")
        printf("");
    // 10. divu
    else if (op == "000000" && funct == "011011")
        printf("");
    // 11. mult
    else if (op == "000000" && funct == "011000")
        printf("");
    // 12. multu
    else if (op == "000000" && funct == "011001")
        printf("");
    // 13. mul
    else if (op == "011100" && funct == "000010")
        printf("");
    // 14. madd
    else if (op == "011100" && funct == "000000")
        printf("");
    // 15. msub
    else if (op == "011100" && funct == "000100")
        printf("");
    // 16. maddu
    else if (op == "011100" && funct == "000001")
        printf("");
    // 17.msubu
    else if (op == "011100" && funct == "000101")
        printf("");
    // 18.nor
    else if (op == "000000" && funct == "100111")
        printf("");
    // 19.or
    else if (op == "000000" && funct == "100101")
        printf("");
    // 20.ori
    else if (op == "001101")
        printf("");
    // 21.sll
    else if (op == "000000" && funct == "000000")
        printf("");
    // 22.sllv
    else if (op == "000000" && funct == "000100")
        printf("");
    // 23.sra
    else if (op == "000000" && funct == "000011")
        printf("");
    // 24.srav
    else if (op == "000000" && funct == "000111")
        printf("");
    // 25.srl
    else if (op == "000000" && funct == "000010")
        printf("");
    // 26.srlv
    else if (op == "000000" && funct == "000110")
        printf("");
    // 27.sub
    else if (op == "000000" && funct == "100010")
        printf("");
    // 28.subu
    else if (op == "000000" && funct == "100011")
        printf("");
    // 29.xor
    else if (op == "000000" && funct == "100110")
        printf("");
    // 30.xori
    else if (op == "001110")
        printf("");
    // 31.lui
    else if (op == "001111")
        printf("");
    // 32.slt
    else if (op == "000000" && funct == "101010")
        printf("");
    // 33.sltu
    else if (op == "000000" && funct == "101011")
        printf("");
    // 34.slti
    else if (op == "001010")
        printf("");
    // 35.sltiu
    else if (op == "001011")
        printf("");
    // 36.beq
    else if (op == "000100")
        printf("");
    // 37.bgez
    else if (op == "000001" && rt_str == "00001")
        printf("");
    // 38.bgezal
    else if (op == "000001" && rt_str == "10001")
        printf("");
    // 39.bgtz
    else if (op == "000111" && rt_str == "00000")
        printf("");
    // 40.blez
    else if (op == "000110" && rt_str == "00000")
        printf("");
    // 41.bltzal
    else if (op == "000001" && rt_str == "10000")
        printf("");
    // 42.bltz
    else if (op == "000001" && rt_str == "00000")
        printf("");
    // 43.bne
    else if (op == "000101")
        printf("");
    // 44.j
    else if (op == "000010")
        printf("");
    // 45.jal
    else if (op == "000011")
        printf("");
    // 46.jalr
    else if (op == "000000" && funct == "001001")
        printf("");
    // 47.jr
    else if (op == "000000" && funct == "001000")
        printf("");
    // 48.teq
    else if (op == "000000" && funct == "110100")
        printf("");
    // 49.teqi
    else if (op == "000001" && rt_str == "01100")
        printf("");
    // 50.tne
    else if (op == "000000" && funct == "110110")
        printf("");
    // 51.tnei
    else if (op == "000001" && rt_str == "01110")
        printf("");
    // 52.tge
    else if (op == "000000" && funct == "110000")
        printf("");
    // 53.tgeu
    else if (op == "000000" && funct == "110001")
        printf("");
    // 54.tgei
    else if (op == "000001" && rt_str == "01000")
        printf("");
    // 55.tgeiu
    else if (op == "000001" && rt_str == "01001")
        printf("");
    // 56.tlt
    else if (op == "000000" && funct == "110010")
        printf("");
    // 57.tltu
    else if (op == "000000" && funct == "110011")
        printf("");
    // 58.tlti
    else if (op == "000001" && rt_str == "01010")
        printf("");
    // 59.tltiu
    else if (op == "000001" && rt_str == "01011")
        printf("");
    // 60.lb
    else if (op == "100000")
        printf("");
    // 61.lbu
    else if (op == "100100")
        printf("");
    // 62.lh
    else if (op == "100001")
        printf("");
    // 63.lhu
    else if (op == "100101")
        printf("");
    // 64.lw
    else if (op == "100011")
        printf("");
    // 65.lwl
    else if (op == "100010")
        printf("");
    // 66.lwr
    else if (op == "100110")
        printf("");
    // 67.ll
    else if (op == "110000")
        printf("");
    // 68.sb
    else if (op == "101000")
        printf("");
    // 69.sh
    else if (op == "101001")
        printf("");
    // 70.sw
    else if (op == "101011")
        printf("");
    // 71.swl
    else if (op == "101010")
        printf("");
    // 72.swr
    else if (op == "101110")
        printf("");
    // 73.sc
    else if (op == "111000")
        printf("");
    // 74.mfhi
    else if (op == "000000" && funct == "010000")
        printf("");
    // 75.mflo
    else if (op == "000000" && funct == "010010")
        printf("");
    // 76.mthi
    else if (op == "000000" && funct == "010001")
        printf("");
    // 77.mtlo
    else if (op == "000000" && funct == "010011")
        printf("");
    // 78.syscall
    else if (op == "000000" && funct == "001100")
        syscall_toExecute(v0_reg, a0_reg);
    else {
        cout << "Unrecognized instruction in `simulateToExecute()`" << endl;
        throw;
    }
}
