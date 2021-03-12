/*
 * This file contains functions related to simulator.
 */

#include <bitset>
using namespace std;


void startSimulator(){
    // Dynamically allocate 6MB to simulate memory
    uint8_t * memoryStart = new uint8_t [6 * 1024 * 1024];





    // free memory
    delete [] memoryStart ;
}