.text
.globl main

main:

li a0, 5
call fib
#call recurse

li a7, 4 #display output
la a0, message2
ecall
li a7, 1
ecall

li a7, 10 #terminate
ecall




fib:
li t0, 1
bgt a0, t0, recurse
ret

recurse:
addi sp, sp, -12
sw ra, 0 (sp)
sw a0, 4(sp)

addi a0, a0, -1
call fib
sw a0, 8(sp)

lw a0, 4(sp)
addi a0, a0, -2
call fib

lw t0, 8(sp)
add a0, a0, t0

lw ra, 0 (sp)
addi sp, sp, 12
ret

.data
message2: .string "Fibonacci of the fifth number is : "