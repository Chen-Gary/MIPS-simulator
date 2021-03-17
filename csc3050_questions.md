`shamt` ---- signed or unsigned?????



$gp



$sp

```
"For example, the stack pointer register, $sp, should always store the current stack top. You should initialize it with a value of 1000000_hex."
```

$fp





```
*For jumping and branching instructions, you need to support both labels and addresses
(offsets).
```



*For instructions with trap (i.e. ==**overflow trap**==), print out the error message and terminate the
program execution.

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

delete `protection` in `simulator_draft.hpp`

---

// i do not know

```

```



---

what I can use in `simulator_instruction_functions.hpp`

```c++
uint32_t* PC_realAddr

uint32_t* rs
uint32_t* rt
uint32_t* rd

int32_t imm_signExtended
uint32_t imm_zeroExtended

uint32_t* v0_reg
uint32_t* a0_reg

uint32_t* ra_reg
    
uint32_t* hi_reg
uint32_t* lo_reg

uint32_t shamt
uint32_t target
    
uint32_t* textSegmentStart
```





