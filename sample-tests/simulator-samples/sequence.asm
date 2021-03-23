.text
addi $v0, $zero, 5
syscall
add $v1, $zero, $v0
addi $s0, $zero, 0
addi $s1, $zero, 0
Loop:
beq $v1, $zero, Exit
addi $v1, $v1, -1
addi $v0, $zero, 5
syscall
add $s0, $s0, $v0
bltzal $s0,  RESET
sub $t0, $s0, $s1
bgezal $t0, UPDATE
j Loop
RESET:
addi $s0, $zero, 0
jr $ra
UPDATE:
addi $s1, $s0, 0
jr $ra
Exit:
addi $v0, $zero, 1
add $a0, $zero, $s1
syscall
addi $v0, $zero, 10
syscall