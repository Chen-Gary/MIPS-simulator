/*
 * This file contains the simulation functions for each instruction.
 */


// 1. add
void add_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = *rs + *rt;

    //(OVERFLOW TRAP)
}
// 2. addu
void addu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = *rs + *rt;
}
// 3. addi
void addi_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended){
    *rt = *rs + imm_signExtended;

    //(OVERFLOW TRAP)
}
// 4. addiu
void addiu_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended){
    *rt = *rs + imm_signExtended;
}
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
void syscall_toExecute(uint32_t* v0_reg, uint32_t* a0_reg){
    uint32_t v0_value = *v0_reg;

    switch (v0_value) {
        case 1: // print_int
            printf("%d", *a0_reg);
            break;
        case 4:
            // print_string
            break;
        case 5: // read_int
            int input;
            cin >> input;
            *v0_reg = input;
            break;
        case 8:
            // read_string
            break;
        case 9:
            // sbrk
            break;
        case 10: // exit
            exit(EXIT_SUCCESS);
            break;
        case 11:
            // print_char
            break;
        case 12:
            // read_char
            break;
        case 13:
            // open
            break;
        case 14:
            // read
            break;
        case 15:
            // write
            break;
        case 16:
            // close
            break;
        case 17:
            // exit2
            break;
        default:
            cout << "Error in syscall instruction: invalid value in $v0!!!" << endl;
            throw;
    }
}