li a7, 4
la a0, string1
ecall


li a7, 4
la a0, string2
ecall



addi a0, zero,10 #1st parameter
addi a1, zero, 15 #2nd parameter
call sum

addi s2, a0, 0 # z= x+y
li a7, 4
la a0, string3
ecall
li a7, 1
mv a0, s2
ecall

li a7, 10
ecall

sum:
add a0, a0, a1 #return
ret

.data
string1: .string "First number : 10\n"
string2: .string "Second number : 15 \n"
string3: .string "Their sum is : "
