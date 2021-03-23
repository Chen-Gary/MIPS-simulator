/*
 * This file contains the simulation functions for each instruction.
 */

#include <fcntl.h>
#include <unistd.h>

const uint64_t MAKS_LOWER_32 = 0x00000000ffffffff;


// PC_realAddr - textSegmentStart = PC_fake - 0x400000 (in bytes)
// *ra_reg = PC_fake!!
uint32_t addrReal2Fake(uint32_t* PC_realAddr, uint32_t* textSegmentStart){
    uint32_t offsetIn1Byte = (uint8_t*) PC_realAddr - (uint8_t*) textSegmentStart;
    uint32_t fakeAddr = offsetIn1Byte + 0x400000;
    return fakeAddr;
}

uint32_t* addrFake2Real(uint32_t fakeAddr, uint32_t* textSegmentStart){
    uint32_t offsetIn1Byte = fakeAddr - 0x400000;
    uint8_t* textSegmentStartByte = (uint8_t*) textSegmentStart;
    uint8_t* realAddrByte = textSegmentStartByte + offsetIn1Byte;
    return (uint32_t*) realAddrByte;
}


// 1. add
void add_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = (int32_t) (*rs) + (int32_t) (*rt);

    if ( ((*rs)&0x80000000) == ((*rt)&0x80000000) && ((*rd)&0x80000000) != ((*rt)&0x80000000) ){
        cout << "add_toExecute(): OVERFLOW TRAP" << endl;
        throw;
    }
}
// 2. addu
void addu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    *rd = *rs + *rt;
}
// 3. addi
void addi_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended){
    *rt = (int32_t) (*rs) + imm_signExtended;

    if ( ((*rs)&0x80000000) == (imm_signExtended&0x80000000) && ((*rt)&0x80000000) != ((*rs)&0x80000000) ){
        cout << "addi_toExecute(): OVERFLOW TRAP" << endl;
        throw;
    }
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
    // ref: https://www.geeksforgeeks.org/number-of-leading-zeros-in-binary-representation-of-a-given-number/
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
    // check overflow ???
    if (*rt == 0){
        cout << "div_toExecute(): OVERFLOW TRAP - divide by zero" << endl;
        throw;
    }

    // *rs / *rt
    *lo_reg = (int32_t) (*rs) / (int32_t) (*rt); // quotient
    *hi_reg = (int32_t) (*rs) % (int32_t) (*rt); // remainder
}
// 10. divu
void divu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    // check overflow ???
    if (*rt == 0){
        cout << "div_toExecute(): OVERFLOW TRAP - divide by zero" << endl;
        throw;
    }

    *lo_reg = (*rs) / (*rt); // quotient
    *hi_reg = (*rs) % (*rt); // remainder
}
// 11. mult
void mult_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    int64_t x = (int32_t) *rs;
    int64_t y = (int32_t) *rt;
    int64_t result = x * y;

    // check overflow
    if (x != 0 && y != 0 && result / x != y){
        cout << "mult_toExecute(): OVERFLOW TRAP" << endl;
        throw;
    }

    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 12. multu
void multu_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    uint64_t x = *rs;
    uint64_t y = *rt;
    uint64_t result = x * y;

    // check overflow
    if (x != 0 && y != 0 && result / x != y){
        cout << "mult_toExecute(): OVERFLOW TRAP" << endl;
        throw;
    }

    *lo_reg = result & MAKS_LOWER_32;
    *hi_reg = (result >> 32) & MAKS_LOWER_32;
}
// 13. mul
void mul_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* rd){
    int64_t x = (int32_t) *rs;
    int64_t y = (int32_t) *rt;
    int64_t result = x * y;

    *rd = result & MAKS_LOWER_32;
}
// 14. madd
void madd_toExecute(uint32_t* rs, uint32_t* rt, uint32_t* hi_reg, uint32_t* lo_reg){
    int64_t x = (int32_t) *rs;
    int64_t y = (int32_t) *rt;
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
    int64_t x = (int32_t) *rs;
    int64_t y = (int32_t) *rt;
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
    int32_t x = (int32_t) *rs;
    int32_t y = (int32_t) *rt;
    int32_t result = x - y;
    *rd = result;

    // check overflow
    if ((result<x) != (y>0)){
        cout << "sub_toExecute(): OVERFLOW TRAP" << endl;
        throw;
    }
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
void bgezal_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr, uint32_t* ra_reg, uint32_t* textSegmentStart){
    // al: and link
    // Save the address of the next instruction in register 31 ,i.e. ra_reg
    *ra_reg = addrReal2Fake(PC_realAddr, textSegmentStart);

    int32_t rs_signed = *rs;
    if (rs_signed >= 0)
        PC_realAddr += imm_signExtended;
}
// 39.bgtz
void bgtz_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr){
    int32_t rs_signed = *rs;
    if (rs_signed > 0)
        PC_realAddr += imm_signExtended;
}
// 40.blez
void blez_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr){
    int32_t rs_signed = *rs;
    if (rs_signed <= 0)
        PC_realAddr += imm_signExtended;
}
// 41.bltzal
void bltzal_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr, uint32_t* ra_reg, uint32_t* textSegmentStart){
    // al
    *ra_reg = addrReal2Fake(PC_realAddr, textSegmentStart);

    int32_t rs_signed = *rs;
    if (rs_signed < 0)
        PC_realAddr += imm_signExtended;
}
// 42.bltz
void bltz_toExecute(uint32_t* rs, int32_t imm_signExtended, uint32_t* &PC_realAddr){
    int32_t rs_signed = *rs;
    if (rs_signed < 0)
        PC_realAddr += imm_signExtended;
}
// 43.bne
void bne_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* &PC_realAddr){
    if (*rs != *rt)
        PC_realAddr += imm_signExtended;
}
// 44.j
void j_toExecute(uint32_t target, uint32_t* &PC_realAddr, uint32_t* textSegmentStart){
    uint32_t fakeAddrCurrent = addrReal2Fake(PC_realAddr, textSegmentStart);
    uint32_t fakeAddrTarget = (fakeAddrCurrent & 0xf0000000) | (target << 2);
    PC_realAddr = addrFake2Real(fakeAddrTarget, textSegmentStart);
}
// 45.jal
void jal_toExecute(uint32_t target, uint32_t* &PC_realAddr, uint32_t* textSegmentStart, uint32_t* ra_reg){
    uint32_t fakeAddrCurrent = addrReal2Fake(PC_realAddr, textSegmentStart);
    uint32_t fakeAddrTarget = (fakeAddrCurrent & 0xf0000000) | (target << 2);
    PC_realAddr = addrFake2Real(fakeAddrTarget, textSegmentStart);

    // al
    *ra_reg = fakeAddrCurrent;
}
// 46.jalr
void jalr_toExecute(uint32_t* rs, uint32_t* rd, uint32_t* &PC_realAddr, uint32_t* textSegmentStart){
    // al
    uint32_t fakeAddrCurrent = addrReal2Fake(PC_realAddr, textSegmentStart);
    *rd = fakeAddrCurrent;

    uint32_t fakeAddrTarget = *rs;
    PC_realAddr = addrFake2Real(fakeAddrTarget, textSegmentStart);
}
// 47.jr
void jr_toExecute(uint32_t* rs, uint32_t* &PC_realAddr, uint32_t* textSegmentStart){
    uint32_t fakeAddrTarget = *rs;
    PC_realAddr = addrFake2Real(fakeAddrTarget, textSegmentStart);
}
// 48.teq
void teq_toExecute(uint32_t* rs, uint32_t* rt){
    if (*rs == *rt){
        cout << "Trap exception: teq rs == rt." << endl;
        throw;
    }
}
// 49.teqi
void teqi_toExecute(uint32_t* rs, int32_t imm_signExtended){
    int32_t rs_signed = *rs;
    if (rs_signed == imm_signExtended){
        cout << "Trap exception: teqi rs == imm." << endl;
        throw;
    }
}
// 50.tne
void tne_toExecute(uint32_t* rs, uint32_t* rt){
    if (*rs != *rt){
        cout << "Trap exception: tne rs != rt." << endl;
        throw;
    }
}
// 51.tnei
void tnei_toExecute(uint32_t* rs, int32_t imm_signExtended){
    int32_t rs_signed = *rs;
    if (rs_signed != imm_signExtended){
        cout << "Trap exception: tnei rs != imm." << endl;
        throw;
    }
}
// 52.tge
void tge_toExecute(uint32_t* rs, uint32_t* rt){
    int32_t rs_signed = *rs;
    int32_t rt_signed = *rt;

    if (rs_signed >= rt_signed){
        cout << "Trap exception: tge rs >= rt." << endl;
        throw;
    }
}
// 53.tgeu
void tgeu_toExecute(uint32_t* rs, uint32_t* rt){
    if (*rs >= *rt){
        cout << "Trap exception: tgeu rs >= rt." << endl;
        throw;
    }
}
// 54.tgei
void tgei_toExecute(uint32_t* rs, int32_t imm_signExtended){
    int32_t rs_signed = *rs;
    if (rs_signed >= imm_signExtended){
        cout << "Trap exception: tgei rs >= imm." << endl;
        throw;
    }
}
// 55.tgeiu
void tgeiu_toExecute(uint32_t* rs, int32_t imm_signExtended){
    if (*rs >= imm_signExtended){
        cout << "Trap exception: tgeiu rs >= imm." << endl;
        throw;
    }
}
// 56.tlt
void tlt_toExecute(uint32_t* rs, uint32_t* rt){
    int32_t rs_signed = *rs;
    int32_t rt_signed = *rt;

    if (rs_signed < rt_signed){
        cout << "Trap exception: tlt rs < rt." << endl;
        throw;
    }
}
// 57.tltu
void tltu_toExecute(uint32_t* rs, uint32_t* rt){
    if (*rs < *rt){
        cout << "Trap exception: tltu rs < rt." << endl;
        throw;
    }
}
// 58.tlti
void tlti_toExecute(uint32_t* rs, int32_t imm_signExtended){
    int32_t rs_signed = *rs;
    if (rs_signed < imm_signExtended){
        cout << "Trap exception: tlti rs < imm." << endl;
        throw;
    }
}
// 59.tltiu
void tltiu_toExecute(uint32_t* rs, int32_t imm_signExtended){
    if (*rs < imm_signExtended){
        cout << "Trap exception: tltiu rs < imm." << endl;
        throw;
    }
}

// Note that the unit of `offset` is byte!!
// e.g. A[8] --> lw $t0, 32($s3)
// Here 32 = 4 * 8

// 60.lb
void lb_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    int8_t* realAddrByte = (int8_t*) realAddr;
    int32_t result = *realAddrByte; // sign-extended
    *rt = result;
}
// 61.lbu
void lbu_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    uint8_t* realAddrByte = (uint8_t*) realAddr;
    uint32_t result = *realAddrByte; // unsign-extended
    *rt = result;
}
// 62.lh
void lh_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    int16_t* realAddrHalf = (int16_t*) realAddr;
    int32_t result = *realAddrHalf; // sign-extended
    *rt = result;
}
// 63.lhu
void lhu_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    uint16_t* realAddrByte = (uint16_t*) realAddr;
    uint32_t result = *realAddrByte; // unsign-extended
    *rt = result;
}
// 64.lw
void lw_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    *rt = *realAddr;
}
// 65.lwl
void lwl_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint8_t* destination = (uint8_t*) rt;

    uint32_t fakeAddr = (*rs) + imm_signExtended;
    do {
        uint8_t* realAddr = (uint8_t*) addrFake2Real(fakeAddr, textSegmentStart);
        *destination = *realAddr;
        destination++;
        fakeAddr++;
    } while (fakeAddr%4 != 0);
}
// 66.lwr
void lwr_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint8_t* destination = (uint8_t*) rt;
    destination += 3;

    uint32_t fakeAddr = (*rs) + imm_signExtended;
    do {
        uint8_t* realAddr = (uint8_t*) addrFake2Real(fakeAddr, textSegmentStart);
        *destination = *realAddr;
        destination--;
        fakeAddr--;
    } while ((fakeAddr+1)%4 != 0);
}
// 67.ll
// Here `ll` is regarded as `lw`.
void ll_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    *rt = *realAddr;
}
// 68.sb
void sb_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    uint8_t* realAddrByte = (uint8_t*) realAddr;
    *realAddrByte = (uint8_t) ((*rt) & 0x000000ff);
}
// 69.sh
void sh_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    uint16_t* realAddrHalf = (uint16_t*) realAddr;
    *realAddrHalf = (uint16_t) ((*rt) & 0x0000ffff);
}
// 70.sw
void sw_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    *realAddr = *rt;
}
// 71.swl
// I do not know
void swl_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint8_t* source = (uint8_t*) rt;

    uint32_t fakeAddr = (*rs) + imm_signExtended;
    do {
        uint8_t* realAddr = (uint8_t*) addrFake2Real(fakeAddr, textSegmentStart);
        *realAddr = *source;
        source++;
        fakeAddr++;
    } while (fakeAddr%4 != 0);
}
// 72.swr
// I do not know
void swr_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint8_t* source = (uint8_t*) rt;
    source += 3;

    uint32_t fakeAddr = (*rs) + imm_signExtended;
    do {
        uint8_t* realAddr = (uint8_t*) addrFake2Real(fakeAddr, textSegmentStart);
        *realAddr = *source;
        source--;
        fakeAddr--;
    } while ((fakeAddr+1)%4 != 0);
}
// 73.sc
// Here `sc` is regarded as `sw`.
void sc_toExecute(uint32_t* rs, uint32_t* rt, int32_t imm_signExtended, uint32_t* textSegmentStart){
    uint32_t fakeAddr = (*rs) + imm_signExtended;
    uint32_t* realAddr = addrFake2Real(fakeAddr, textSegmentStart);

    *realAddr = *rt;
}
// 74.mfhi
void mfhi_toExecute(uint32_t* rd, uint32_t* hi_reg){
    *rd = *hi_reg;
}
// 75.mflo
void mflo_toExecute(uint32_t* rd, uint32_t* lo_reg){
    *rd = *lo_reg;
}
// 76.mthi
void mthi_toExecute(uint32_t* rs, uint32_t* hi_reg){
    *hi_reg = *rs;
}
// 77.mtlo
void mtlo_toExecute(uint32_t* rs, uint32_t* lo_reg){
    *lo_reg = *rs;
}
// 78.syscall
void syscall_toExecute(uint32_t* v0_reg, uint32_t* a0_reg, uint32_t* a1_reg, uint32_t* a2_reg, uint32_t* textSegmentStart, uint8_t* &dynamicDataSegmentCurrent){
    uint32_t v0_value = *v0_reg;

    // case 4
    uint32_t* realAddr = addrFake2Real(*a0_reg, textSegmentStart);
    char* ch_ptr = (char*) realAddr;

    // case 5
    int input_int;

    // case 8
    uint32_t buffer_a0 = *a0_reg;
    char* realBufferAddr_a0 = (char*) addrFake2Real(buffer_a0, textSegmentStart);
    uint32_t length_a1 = *a1_reg;

    // case 12
    char input_char;

    //case 14, 15, 16
    uint32_t file_descriptor = *a0_reg;
    uint32_t buffer_a1 = *a1_reg;
    uint8_t* realBufferAddr_a1 = (uint8_t*) addrFake2Real(buffer_a1, textSegmentStart);
    uint32_t length_a2 = *a2_reg;

    switch (v0_value) {
        case 1: // print_int // change IO
            printf("%d", *a0_reg);
            break;
        case 4: // print_string // change IO
            while ((*ch_ptr) != '\0'){
                printf("%c", *ch_ptr);
                ch_ptr++;
            }
            break;
        case 5: // read_int // change IO
            cin >> input_int;
            *v0_reg = input_int;
            break;
        case 8: // read_string // change IO
            //... (not implemented)
            break;
        case 9: // sbrk
            *v0_reg = addrReal2Fake( (uint32_t*) dynamicDataSegmentCurrent, textSegmentStart);
            dynamicDataSegmentCurrent += (*a0_reg);
            break;
        case 10: // exit
            exit(EXIT_SUCCESS);
            break;
        case 11: // print_char // change IO
            printf("%c", ((*a0_reg) & 0x000000ff));
            break;
        case 12: // read_char // change IO
            cin >> input_char;
            *v0_reg = (uint8_t) input_char;
            break;
        case 13: // open
            *v0_reg = open( (char*) a0_reg, (int) *a1_reg, (mode_t) *a2_reg);
            break;
        case 14: // read
            *v0_reg = read(file_descriptor, realBufferAddr_a1, length_a2);
            break;
        case 15: // write
            *v0_reg = write(file_descriptor, realBufferAddr_a1, length_a2);
            break;
        case 16: // close
            close(file_descriptor);
            break;
        case 17: // exit2
            exit(2);
            break;
        default:
            cout << "Error in syscall instruction: invalid value in $v0!!!" << endl;
            throw;
    }
}
