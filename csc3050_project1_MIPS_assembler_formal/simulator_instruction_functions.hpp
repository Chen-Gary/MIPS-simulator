/*
 * This file contains the simulation functions for each instruction.
 */

const uint64_t MAKS_LOWER_32 = 0xffffffff;

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
void and_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rs) & (*rt);
}
// 6. andi
void andi_toExecute(uint32_t* rs, uint32_t* rt, uint32_t imm_zeroExtended){
    *rt = (*rs) & imm_zeroExtended;
}
// 7. clo
void clo_toExecute(uint32_t* rs, uint32_t* rd){
    uint32_t x = *rs;
    int total_bits = sizeof(x) * 8;
    int res = 0;
    while ( x & (1 << (total_bits - 1)) ) {
        x = (x << 1);
        res++;
    }
    *rd = res;
}
// 8. clz
void clz_toExecute(uint32_t* rs, uint32_t* rd){
    // https://www.geeksforgeeks.org/number-of-leading-zeros-in-binary-representation-of-a-given-number/
    uint32_t x = *rs;
    int total_bits = sizeof(x) * 8;
    int res = 0;
    while ( !(x & (1 << (total_bits - 1))) ){
        x = (x << 1);
        res++;
    }
    *rd = res;
}
// 9. div
void div_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    // *rs / *rt
    *lo_reg = (*rs) / (*rt); // quotient
    *hi_reg = (*rs) % (*rt); // remainder

    //(OVERFLOW TRAP)
}
// 10. divu
void divu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    *lo_reg = (*rs) / (*rt); // quotient
    *hi_reg = (*rs) % (*rt); // remainder
}
// 11. mult
void mult_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    int64_t x = *rs;
    int64_t y = *rt;
    int64_t result = x * y;
    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 12. multu
void multu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    uint64_t x = *rs;
    uint64_t y = *rt;
    uint64_t result = x * y;
    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 13. mul
void mul_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rs) * (*rt);
}
// 14. madd
void madd_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    int64_t x = *rs;
    int64_t y = *rt;
    int64_t result = x * y;

    uint64_t hi = *hi_reg;
    uint64_t lo = *lo_reg;
    int64_t hiLo = (hi << 32) | lo;
    result += hiLo;

    // store back to $hi, $lo
    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 15. msub
void msub_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    int64_t x = *rs;
    int64_t y = *rt;
    int64_t result = x * y;

    uint64_t hi = *hi_reg;
    uint64_t lo = *lo_reg;
    int64_t hiLo = (hi << 32) | lo;
    result = hiLo - result;

    // store back to $hi, $lo
    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 16. maddu
void maddu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    uint64_t x = *rs;
    uint64_t y = *rt;
    uint64_t result = x * y;

    uint64_t hi = *hi_reg;
    uint64_t lo = *lo_reg;
    uint64_t hiLo = (hi << 32) | lo;
    result += hiLo;

    // store back to $hi, $lo
    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 17.msubu
void msubu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    uint64_t x = *rs;
    uint64_t y = *rt;
    uint64_t result = x * y;

    uint64_t hi = *hi_reg;
    uint64_t lo = *lo_reg;
    uint64_t hiLo = (hi << 32) | lo;
    result = hiLo - result;

    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 18.nor
void nor_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = ~ ((*rs) | (*rt));
}
// 19.or
void or_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rs) | (*rt);
}
// 20.ori
void ori_toExecute(uint32_t* rs, uint32_t* rt, uint32_t imm_zeroExtended){
    *rt = (*rs) | imm_zeroExtended;
}
// 21.sll
void sll_toExecute(uint32_t* rt, uint32_t* rd, uint32_t shamt){
    *rd = (*rt) << shamt;
}
// 22.sllv
void sllv_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rt) << (*rs);
}
// 23.sra
void sra_toExecute(uint32_t* rt, uint32_t* rd, uint32_t shamt){
    // sign extension
    int32_t toShift = *rt;
    *rd = toShift >> shamt;
}
// 24.srav
void srav_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    // sign extension
    int32_t toShift = *rt;
    *rd = toShift >> (*rs);
}
// 25.srl
void srl_toExecute(uint32_t* rt, uint32_t* rd, uint32_t shamt){
    // unsigned extension
    *rd = (*rt) >> shamt;
}
// 26.srlv
void srlv_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    // unsigned extension
    *rd = (*rt) >> (*rs);
}
// 27.sub
void sub_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rs) - (*rt);

    //(OVERFLOW TRAP)
}
// 28.subu
void subu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rs) - (*rt);
}
// 29.xor
void xor_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (*rs) ^ (*rt);
}
// 30.xori
void xori_toExecute(uint32_t* rs, uint32_t* rt, uint32_t imm_zeroExtended){
    *rt = (*rs) ^ imm_zeroExtended;
}
// 31.lui
void lui_toExecute(uint32_t* rt, uint32_t imm_zeroExtended){
    *rt = (imm_zeroExtended << 16);
}
// 32.slt
void slt_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    int32_t rs_signed = *rs;
    int32_t rt_signed = *rt;

    if (rs_signed < rt_signed)
        *rd = 1;
    else
        *rd = 0;
}
// 33.sltu
void sltu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    if (*rs < *rt)
        *rd = 1;
    else
        *rd = 0;
}
// 34.slti
void slti_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended){
    int32_t rs_signed = *rs;

    if (rs_signed < imm_signExtended)
        *rt = 1;
    else
        *rt = 0;
}
// 35.sltiu
void sltiu_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended){
    if (*rs < imm_signExtended)
        *rt = 1;
    else
        *rt = 0;
}

// Note that offset = imm_signExtended
// PC = PC + 4 + offset*4
// 36.beq
void beq_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* &PC_realAddr){
    if (*rs == *rt)
        PC_realAddr += imm_signExtended;
}
// 37.bgez
void bgez_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr){
    int32_t rs_signed = *rs;
    if (rs_signed >= 0)
        PC_realAddr += imm_signExtended;
}
// 38.bgezal
void bgezal_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr, uint32_t* ra_reg, const uint32_t* textSegmentStart){
    int32_t rs_signed = *rs;
    if (rs_signed >= 0)
        PC_realAddr += imm_signExtended;
    // al: and link
    // Save the address of the next instruction in register 31 ,i.e. ra_reg
    // PC_realAddr - textSegmentStart = PC_fake - 0x400000 (in bytes)
    // *ra_reg = PC_fake!!
    uint32_t offsetIn4Byte = PC_realAddr - textSegmentStart;
    *ra_reg = offsetIn4Byte * 4 + 0x400000;
}
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
        case 1: // print_int // change IO
            printf("%d", *a0_reg);
            break;
        case 4:
            // print_string
            break;
        case 5: // read_int // change IO
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