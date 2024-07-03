.text
.globl main

main:

li a7, 4
la a0, message1
ecall

addi s0, zero, 10

add s1, zero, zero #i = 0
loop: beq s0, zero, Exit
addi s1, s1, 1 # i = i+1
srai s0, s0, 1 # i = i/2
j loop

Exit: #Display output
li a7, 4
la a0, message2
ecall
li a7, 1
mv a0, s1
ecall

li a7, 10 #terminate
ecall



.data
message1: .string "The number is 10\n"
message2: .string "The number of times we incremented the counter before it reached 0 was: "
