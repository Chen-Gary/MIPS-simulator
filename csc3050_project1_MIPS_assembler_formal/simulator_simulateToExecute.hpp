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

// target in J-type
uint32_t zeroExtendedTarget(uint32_t instruction){
    uint32_t target = (instruction << 6) >> 6;
    return target;
}

// Note that in this implementation we do not check which type (R, I or J) the current instruction is.
// Instead we directly use `op` and `funct` to identify which instruction PC_realAddr currently points to
void simulateToExecute(uint32_t* &PC_realAddr, const map<string, uint32_t*> & str2SimulatedRegister, uint32_t* textSegmentStart){
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

    uint32_t* ra_reg = str2SimulatedRegister.find("11111")->second;

    uint32_t* hi_reg = str2SimulatedRegister.find("hi_reg")->second;
    uint32_t* lo_reg = str2SimulatedRegister.find("lo_reg")->second;

    // get `shamt`
    string shamt_str = instructionStr.substr(21, 5);
    uint32_t shamt = bitset<5>(shamt_str).to_ulong();      // regard shamt as a 5-bit unsigned number
    // get `target`
    uint32_t target = zeroExtendedTarget(instruction);

    // useless codes here
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
        and_toExecute(rs, rt, rd);
    // 6. andi
    else if (op == "001100")
        andi_toExecute(rs, rt, imm_zeroExtended);
    // 7. clo
    else if (op == "011100" && funct == "100001")
        clo_toExecute(rs, rd);
    // 8. clz
    else if (op == "011100" && funct == "100000")
        clz_toExecute(rs, rd);
    // 9. div
    else if (op == "000000" && funct == "011010")
        div_toExecute(rs, rt, hi_reg, lo_reg);
    // 10. divu
    else if (op == "000000" && funct == "011011")
        divu_toExecute(rs, rt, hi_reg, lo_reg);
    // 11. mult
    else if (op == "000000" && funct == "011000")
        mult_toExecute(rs, rt, hi_reg, lo_reg);
    // 12. multu
    else if (op == "000000" && funct == "011001")
        multu_toExecute(rs, rt, hi_reg, lo_reg);
    // 13. mul
    else if (op == "011100" && funct == "000010")
        mul_toExecute(rs, rt, rd);
    // 14. madd
    else if (op == "011100" && funct == "000000")
        madd_toExecute(rs, rt, hi_reg, lo_reg);
    // 15. msub
    else if (op == "011100" && funct == "000100")
        msub_toExecute(rs, rt, hi_reg, lo_reg);
    // 16. maddu
    else if (op == "011100" && funct == "000001")
        maddu_toExecute(rs, rt, hi_reg, lo_reg);
    // 17.msubu
    else if (op == "011100" && funct == "000101")
        msubu_toExecute(rs, rt, hi_reg, lo_reg);
    // 18.nor
    else if (op == "000000" && funct == "100111")
        nor_toExecute(rs, rt, rd);
    // 19.or
    else if (op == "000000" && funct == "100101")
        or_toExecute(rs, rt, rd);
    // 20.ori
    else if (op == "001101")
        ori_toExecute(rs, rt, imm_zeroExtended);
    // 21.sll
    else if (op == "000000" && funct == "000000")
        sll_toExecute(rt, rd, shamt);
    // 22.sllv
    else if (op == "000000" && funct == "000100")
        sllv_toExecute(rs, rt, rd);
    // 23.sra
    else if (op == "000000" && funct == "000011")
        sra_toExecute(rt, rd, shamt);
    // 24.srav
    else if (op == "000000" && funct == "000111")
        srav_toExecute(rs, rt, rd);
    // 25.srl
    else if (op == "000000" && funct == "000010")
        srl_toExecute(rt, rd, shamt);
    // 26.srlv
    else if (op == "000000" && funct == "000110")
        srlv_toExecute(rs, rt, rd);
    // 27.sub
    else if (op == "000000" && funct == "100010")
        sub_toExecute(rs, rt, rd);
    // 28.subu
    else if (op == "000000" && funct == "100011")
        subu_toExecute(rs, rt, rd);
    // 29.xor
    else if (op == "000000" && funct == "100110")
        xor_toExecute(rs, rt, rd);
    // 30.xori
    else if (op == "001110")
        xori_toExecute(rs, rt, imm_zeroExtended);
    // 31.lui
    else if (op == "001111")
        lui_toExecute(rt, imm_zeroExtended);
    // 32.slt
    else if (op == "000000" && funct == "101010")
        slt_toExecute(rs, rt, rd);
    // 33.sltu
    else if (op == "000000" && funct == "101011")
        sltu_toExecute(rs, rt, rd);
    // 34.slti
    else if (op == "001010")
        slti_toExecute(rs, rt, imm_signExtended);
    // 35.sltiu
    else if (op == "001011")
        sltiu_toExecute(rs, rt, imm_signExtended);
    // 36.beq
    else if (op == "000100")
        beq_toExecute(rs, rt, imm_signExtended, PC_realAddr);
    // 37.bgez
    else if (op == "000001" && rt_str == "00001")
        bgez_toExecute(rs, imm_signExtended, PC_realAddr);
    // 38.bgezal
    else if (op == "000001" && rt_str == "10001")
        bgezal_toExecute(rs, imm_signExtended, PC_realAddr, ra_reg, textSegmentStart);
    // 39.bgtz
    else if (op == "000111" && rt_str == "00000")
        bgtz_toExecute(rs, imm_signExtended, PC_realAddr);
    // 40.blez
    else if (op == "000110" && rt_str == "00000")
        blez_toExecute(rs, imm_signExtended, PC_realAddr);
    // 41.bltzal
    else if (op == "000001" && rt_str == "10000")
        bltzal_toExecute(rs, imm_signExtended, PC_realAddr, ra_reg, textSegmentStart);
    // 42.bltz
    else if (op == "000001" && rt_str == "00000")
        bltz_toExecute(rs, imm_signExtended, PC_realAddr);
    // 43.bne
    else if (op == "000101")
        bne_toExecute(rs, rt, imm_signExtended, PC_realAddr);
    // 44.j
    else if (op == "000010")
        j_toExecute(target, PC_realAddr, textSegmentStart);
    // 45.jal
    else if (op == "000011")
        jal_toExecute(target, PC_realAddr, textSegmentStart, ra_reg);
    // 46.jalr
    else if (op == "000000" && funct == "001001")
        jalr_toExecute(rs, rd, PC_realAddr, textSegmentStart);
    // 47.jr
    else if (op == "000000" && funct == "001000")
        jr_toExecute(rs, PC_realAddr, textSegmentStart);
    // 48.teq
    else if (op == "000000" && funct == "110100")
        teq_toExecute(rs, rt);
    // 49.teqi
    else if (op == "000001" && rt_str == "01100")
        teqi_toExecute(rs, imm_signExtended);
    // 50.tne
    else if (op == "000000" && funct == "110110")
        tne_toExecute(rs, rt);
    // 51.tnei
    else if (op == "000001" && rt_str == "01110")
        tnei_toExecute(rs, imm_signExtended);
    // 52.tge
    else if (op == "000000" && funct == "110000")
        tge_toExecute(rs, rt);
    // 53.tgeu
    else if (op == "000000" && funct == "110001")
        tgeu_toExecute(rs, rt);
    // 54.tgei
    else if (op == "000001" && rt_str == "01000")
        tgei_toExecute(rs, imm_signExtended);
    // 55.tgeiu
    else if (op == "000001" && rt_str == "01001")
        tgeiu_toExecute(rs, imm_signExtended);
    // 56.tlt
    else if (op == "000000" && funct == "110010")
        tlt_toExecute(rs, rt);
    // 57.tltu
    else if (op == "000000" && funct == "110011")
        tltu_toExecute(rs, rt);
    // 58.tlti
    else if (op == "000001" && rt_str == "01010")
        tlti_toExecute(rs, imm_signExtended);
    // 59.tltiu
    else if (op == "000001" && rt_str == "01011")
        tltiu_toExecute(rs, imm_signExtended);
    // 60.lb
    else if (op == "100000")
        lb_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 61.lbu
    else if (op == "100100")
        lbu_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 62.lh
    else if (op == "100001")
        lh_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 63.lhu
    else if (op == "100101")
        lhu_toExecute(rs, rt ,imm_signExtended, textSegmentStart);
    // 64.lw
    else if (op == "100011")
        lw_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 65.lwl
    else if (op == "100010")
        printf("to be implemented\n");
    // 66.lwr
    else if (op == "100110")
        printf("to be implemented\n");
    // 67.ll
    else if (op == "110000")
        ll_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 68.sb
    else if (op == "101000")
        sb_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 69.sh
    else if (op == "101001")
        sh_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 70.sw
    else if (op == "101011")
        sw_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 71.swl
    else if (op == "101010")
        printf("to be implemented\n");
    // 72.swr
    else if (op == "101110")
        printf("to be implemented\n");
    // 73.sc
    else if (op == "111000")
        sc_toExecute(rs, rt, imm_signExtended, textSegmentStart);
    // 74.mfhi
    else if (op == "000000" && funct == "010000")
        mfhi_toExecute(rd, hi_reg);
    // 75.mflo
    else if (op == "000000" && funct == "010010")
        mflo_toExecute(rd, lo_reg);
    // 76.mthi
    else if (op == "000000" && funct == "010001")
        mthi_toExecute(rs, hi_reg);
    // 77.mtlo
    else if (op == "000000" && funct == "010011")
        mtlo_toExecute(rs, lo_reg);
    // 78.syscall
    else if (op == "000000" && funct == "001100")
        syscall_toExecute(v0_reg, a0_reg, textSegmentStart);
    else {
        cout << "Unrecognized instruction in `simulateToExecute()`" << endl;
        throw;
    }
}
