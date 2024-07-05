.text
.globl main
main: 

#Entering start number
li a7,4
la a0, message1
ecall
addi s0, zero, 2 #first number

li a7,4
la a0, message2
ecall
addi s1, zero, 10 #second number

li a7,4
la a0, message3
ecall
addi s2, zero, 2 # step size

addi s3, s0, 0 #start of loop
addi s4, zero, 0 #sum output
counting:bgt s3,s1, display
add s4,s4,s3
add s3, s3, s2
j counting

#Displaying the result
display: li a7, 4
la a0, message4
ecall
li a7, 1
mv a0,s4
ecall

li a7, 10
ecall
.data
message1: .string "Starting number: 2 \n"
message2: .string "Ending number: 10\n"
message3: .string "Step size: 2 \n"
message4: .string "The sum is: \n"
