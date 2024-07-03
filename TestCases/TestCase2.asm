li a7, 4
la a0, string1
ecall
li a7, 5
ecall
mv s0, a0 #first number

li a7, 4
la a0, string2
ecall
li a7, 5 
ecall
mv s1, a0 #second number


add a0, s0,zero #1st parameter
add a1, s1, zero #2nd parameter
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
string1: .string "Enter first number : "
string2: .string "Enter second number : "
string3: .string "Their sum is : "
