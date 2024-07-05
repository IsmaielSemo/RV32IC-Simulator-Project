.text
.globl main
main:
li a7, 4
la a0, message1
ecall
addi s0, zero, 5


call add #function call

#display part
li a7, 4
la a0, message2
ecall
li a7, 1
mv a0, s0
ecall
li a7, 10 #terminate
ecall


add:
addi sp, sp, -8 #expanding stack by 2 places
sw ra, 0(sp) #store the current address
sw s0, 4(sp) #store n
bne s0, zero, else # if n != 0
j exit
else: addi s0, s0, -1 # n = n-1
call add
lw s1, 4(sp) #load n
add s0,s0,s1 #get n + n-1
j exit

exit:
lw ra, 0(sp)#load original address
addi sp, sp, 8 #pop from stack
ret

.data
message1: .string "The number is: 5 \n"
message2: .string "The output is: "
