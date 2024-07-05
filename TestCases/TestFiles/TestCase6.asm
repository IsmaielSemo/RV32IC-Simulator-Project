.text
.globl main
main:
li a7, 4
la a0, message1
ecall
addi s0, zero, 30 #first number

li a7, 4
la a0, message2
ecall
addi s1, zero, 13 #second number


addi s2, zero, 0 #s2 = ans = 0
addi s3, zero, 0 #s3 = count = 0
addi s5, zero, 1 #to store remainder 1

# m & 2 = m - (m/8 * 8) -> remainder

loop_start:
beq s1,zero, exit
addi s4, s1, 0
andi s4, s4, 1 

 
bne s4,s5, loop_end
sll t2, s0, s3 # n << count
add s2, s2, t2 # ans += n << count

loop_end:
srli s1, s1, 1 #m/2
addi s3, s3, 1 # count ++
j loop_start


#printing the result
exit: li a7,4
la a0, message3
ecall
li a7, 1
mv a0, s2
ecall

li a7, 10
ecall #termination
.data
message1: .string "First unsigned number: 30\n"
message2: .string "Second unsigned number: 13\n"
message3: .string "Their product is: "
