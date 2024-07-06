

.text
.globl main

main:
    # Calculate Fibonacci of 5
    li a0, 5          # Load immediate value 5 into register a0
    call fib          # Call fib function with argument 5

    # Print the result
    li a7, 4          # syscall code for print_string
    la a0, message    # Load address of message into a0
    ecall             # Perform syscall to print the string

    # Print the Fibonacci result
    li a7, 1          # syscall code for print_integer
    mv a0, a1         # Move Fibonacci result to a0 (assuming result is in a1)
    ecall             # Perform syscall to print the integer

    # Exit program
    li a7, 10         # syscall code for exit
    ecall             # Exit the program

# Function to compute Fibonacci
fib:
    # Save registers ra, a0, and t0 on stack
    addi sp, sp, -12  # Adjust stack pointer
    sw ra, 0(sp)      # Save return address
    sw a0, 4(sp)      # Save argument a0
    sw t0, 8(sp)      # Save temporary register t0

    # Base case: if a0 <= 1, return a0
    li t0, 1          # Load immediate value 1 into t0
    ble a0, t0, base_case  # Branch to base_case if a0 <= 1

    # Recursive calls for Fibonacci
    addi a0, a0, -1    # a0 = a0 - 1
    call fib           # Recursive call to fib(a0 - 1)
    mv t0, a1          # Move result of fib(a0 - 1) to t0

    lw a0, 4(sp)       # Restore a0 from stack
    addi a0, a0, -2    # a0 = a0 - 2
    call fib           # Recursive call to fib(a0 - 2)

    add a1, a1, t0     # a1 = a1 + t0 (accumulate result)

    # Restore registers ra, a0, and t0 from stack
    lw t0, 8(sp)       # Restore t0 from stack
    lw a0, 4(sp)       # Restore a0 from stack
    lw ra, 0(sp)       # Restore return address from stack
    addi sp, sp, 12    # Restore stack pointer

    ret                # Return from fib function

base_case:
    mv a1, a0          # Return a0 as the Fibonacci value (base case)

    # Restore registers ra, a0, and t0 from stack
    lw t0, 8(sp)       # Restore t0 from stack
    lw a0, 4(sp)       # Restore a0 from stack
    lw ra, 0(sp)       # Restore return address from stack
    addi sp, sp, 12    # Restore stack pointer

    ret                # Return from fib function
    
.data
message: .string "Fibonacci of the fifth number is: "
