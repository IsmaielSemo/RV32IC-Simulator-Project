.text
.globl main
main:
li a7, 4
la a0, message1
ecall
addi s0, zero, 7 #the unsigned number


call bitC

#Display
li a7,4
la a0, message2
ecall
li a7, 1
mv a0,s0
ecall

li a7, 10 #terminate
ecall


bitC:
addi sp, sp, -8 #extend stack by 2 elements
sw ra, 0(sp) #add address to stack
sw s0, 4(sp) #add og value of x
bne s0, zero, loop #if x != 0
j exit

loop: 
andi s1, s0, 1 #bit = x & 0x1
sw s1, 4(sp) #store it in stack
srai s0, s0, 1 # x >> 1
call bitC
lw s2, 4(sp) 
add s0,s0,s2 
j exit

exit: lw ra, 0(sp) #orriginal address
addi sp, sp, 8 #pop from stack
ret


.data
message1: .string "The unsigned number is: 7 \n"
message2: .string "The output is (number of 1's in 7) : "
