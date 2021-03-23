.data
Letter: .byte 65, 66, 67, 10
Arrow: .asciiz " --> "
.text
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0
addi $s1, $zero, 1
lui $at, 80
lb $s2, 0($at)
lb $s3, 1($at)
lb $s4, 2($at)
HanoiTower:
addi $sp, $sp, -20
sw $s2, 0($sp)
sw $s3, 4($sp)
sw $s4, 8($sp)
sw $ra, 12($sp)
sw $s0, 16($sp)
beq $s0, $s1, Jump_Back
lw $s0, 16($sp)
addu $t2, $zero, $s2
addu $t3, $zero, $s3
addu $t4, $zero, $s4
addu $s2, $zero, $t2
addu $s3, $zero, $t4
addu $s4, $zero, $t3
sub $s0, $s0, $s1
jal HanoiTower
lw $s0, 16($sp)
lw $s2, 0($sp)
lw $s3, 4($sp)
lw $s4, 8($sp)
addi $v0, $zero, 11
addu $a0, $zero, $s2
syscall
addi $v0, $zero, 4
lui $at, 80
addi $a0, $at, 4
syscall
addi $v0, $zero, 11
addu $a0, $zero, $s4
syscall
lui $at, 80
lb $a0, 3($at)
syscall
addu $t2, $zero, $s2
addu $t3, $zero, $s3
addu $t4, $zero, $s4
addu $s2, $zero, $t3
addu $s3, $zero, $t2
addu $s4, $zero, $t4
sub $s0, $s0, $s1
jal HanoiTower
lw $ra, 12($sp)
addi $sp, $sp, 20
beq $ra, $zero, Exit
jr $ra
Jump_Back:
addi $v0, $zero, 11
addu $a0, $zero, $s2
syscall
addi $v0, $zero, 4
lui $at, 80
addi $a0, $at, 4
syscall
addi $v0, $zero,11
addu $a0, $zero, $s4
syscall
lui $at, 80
lb $a0, 3($at)
syscall
lw $ra, 12($sp)
beq $ra, $zero, Exit
addi $sp, $sp, 20
jr $ra
Exit:
addi $v0, $zero, 10
syscall
