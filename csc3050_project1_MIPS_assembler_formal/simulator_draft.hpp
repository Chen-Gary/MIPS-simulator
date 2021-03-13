/*
 * This file contains functions related to simulator.
 */

#include "simulator_simulateToExecute.hpp"
#include <bitset>
#include <vector>
#include <string>
using namespace std;


void startSimulator(const vector<string> instructionsBinary){

    // Dynamically allocate 6MB to simulate memory (RAM)
    uint8_t * memoryStart = new uint8_t [6 * 1024 * 1024];          // fake address 0x400000
    uint32_t * textSegmentStart = (uint32_t*) memoryStart;          // fake address 0x400000 (decimal 4194304)
    uint8_t * staticDataSegmentStart = memoryStart + 1024 * 1024;   // fake address 0x500000 (decimal 5242880)
    uint8_t * dynamicDataSegmentStart;                              // wherever static data section ends
    uint8_t * stackSegmentStart = memoryStart + 6 * 1024 * 1024;    // fake address 0xa00000

    // Simulate registers (the address store in these fake registers are all fake address, instead of real address)
    uint32_t * zero_reg = new uint32_t {0};
    uint32_t * at_reg = new uint32_t {0};
    uint32_t * v0_reg = new uint32_t {0};
    uint32_t * v1_reg = new uint32_t {0};
    uint32_t * a0_reg = new uint32_t {0};
    uint32_t * a1_reg = new uint32_t {0};
    uint32_t * a2_reg = new uint32_t {0};
    uint32_t * a3_reg = new uint32_t {0};
    uint32_t * t0_reg = new uint32_t {0};
    uint32_t * t1_reg = new uint32_t {0};
    uint32_t * t2_reg = new uint32_t {0};
    uint32_t * t3_reg = new uint32_t {0};
    uint32_t * t4_reg = new uint32_t {0};
    uint32_t * t5_reg = new uint32_t {0};
    uint32_t * t6_reg = new uint32_t {0};
    uint32_t * t7_reg = new uint32_t {0};
    uint32_t * s0_reg = new uint32_t {0};
    uint32_t * s1_reg = new uint32_t {0};
    uint32_t * s2_reg = new uint32_t {0};
    uint32_t * s3_reg = new uint32_t {0};
    uint32_t * s4_reg = new uint32_t {0};
    uint32_t * s5_reg = new uint32_t {0};
    uint32_t * s6_reg = new uint32_t {0};
    uint32_t * s7_reg = new uint32_t {0};
    uint32_t * t8_reg = new uint32_t {0};
    uint32_t * t9_reg = new uint32_t {0};
    uint32_t * k0_reg = new uint32_t {0};
    uint32_t * k1_reg = new uint32_t {0};
    uint32_t * gp_reg = new uint32_t {0};//???
    uint32_t * sp_reg = new uint32_t {10485760};// 10485760=0xa00000; "For example, the stack pointer register, $sp, should always store the current stack top. You should initialize it with a value of 1000000_hex."
    uint32_t * fp_reg = new uint32_t {0};//???
    uint32_t * ra_reg = new uint32_t {0};

    // map from binary_string_addr to register (which is uint32_t*)
    const map<string, uint32_t*> str2SimulatedRegister = {
            {"00000", zero_reg},
            {"00001", at_reg},
            {"00010", v0_reg},
            {"00011", v1_reg},
            {"00100", a0_reg},
            {"00101", a1_reg},
            {"00110", a2_reg},
            {"00111", a3_reg},
            {"01000", t0_reg},
            {"01001", t1_reg},
            {"01010", t2_reg},
            {"01011", t3_reg},
            {"01100", t4_reg},
            {"01101", t5_reg},
            {"01110", t6_reg},
            {"01111", t7_reg},
            {"10000", s0_reg},
            {"10001", s1_reg},
            {"10010", s2_reg},
            {"10011", s3_reg},
            {"10100", s4_reg},
            {"10101", s5_reg},
            {"10110", s6_reg},
            {"10111", s7_reg},
            {"11000", t8_reg},
            {"11001", t9_reg},
            {"11010", k0_reg},
            {"11011", k1_reg},
            {"11100", gp_reg},
            {"11101", sp_reg},
            {"11110", fp_reg},
            {"11111", ra_reg},
    };


    // Put the data in .data segment of MIPS file piece by piece in the static data segment.
    // ...


    // Put the assembled machine code in the .text segment of the simulated memory
    // Note that the machine code is store as little endian in this method
    uint32_t * textSegmentCurrent = textSegmentStart;
    for (string machineCodeStr : instructionsBinary){
        *textSegmentCurrent = bitset<32>(machineCodeStr).to_ulong();
        textSegmentCurrent++;
    }


    // Start simulating
    uint32_t * PC_realAddr = textSegmentStart;
    int protection = 0;
    while (protection < 11){
        simulateToExecute(PC_realAddr, str2SimulatedRegister);

        // PC = PC + 4 is calculated inside `simulateToExecute()`
        protection++;
    }
    // debug
    if (protection >= 11)
        cout << "protection exceed!!!" << endl;



    // simulation ends
    // free memory
    delete [] memoryStart;
    delete zero_reg;
    delete at_reg;
    delete v0_reg;
    delete v1_reg;
    delete a0_reg;
    delete a1_reg;
    delete a2_reg;
    delete a3_reg;
    delete t0_reg;
    delete t1_reg;
    delete t2_reg;
    delete t3_reg;
    delete t4_reg;
    delete t5_reg;
    delete t6_reg;
    delete t7_reg;
    delete s0_reg;
    delete s1_reg;
    delete s2_reg;
    delete s3_reg;
    delete s4_reg;
    delete s5_reg;
    delete s6_reg;
    delete s7_reg;
    delete t8_reg;
    delete t9_reg;
    delete k0_reg;
    delete k1_reg;
    delete gp_reg;
    delete sp_reg;
    delete fp_reg;
    delete ra_reg;
}