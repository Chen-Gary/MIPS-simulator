*For instructions with trap (i.e. ==**overflow trap**==), print out the error message and terminate the program execution.

//(OVERFLOW TRAP)

```
add
addi
div
sub
```

---

// change IO (`syscall_toExecute`+ input file)



---

what I can use in `simulator_instruction_functions.hpp`

```c++
uint32_t* PC_realAddr
uint8_t* &dynamicDataSegmentCurrent

uint32_t* rs
uint32_t* rt
uint32_t* rd

int32_t imm_signExtended
uint32_t imm_zeroExtended

uint32_t* v0_reg
uint32_t* a0_reg
uint32_t* a1_reg
uint32_t* a2_reg

uint32_t* ra_reg
    
uint32_t* hi_reg
uint32_t* lo_reg

uint32_t shamt
uint32_t target
    
uint32_t* textSegmentStart
```

---

// I do not know

```
swl
swr

syscall:

```

