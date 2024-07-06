# RV32IC-Simulator-Project
This project will contain all files for Assembly project 1 under Dr Mohamed Shalan.

This project is completed by Ismaiel Sabet, Seif Elansari, and Jana Fadl.

Process:
We first started with taking in the skeleton code, understanding it, and then complete the rest of the R-type insturction set. Then, we implemented the I-type instructions (both opcodes for non-load and load instructions respectively). We then did the B-type instructions and the S-type instructions, before attempting to explore some of the execution part with the R-type and I-type, followed by the B-type, S-type, U-type, and J-type. We then implemented the compressed instructions and tested the simulator.

Expected functionality:
The program will take in (in both terminal and i/o) a file containing 32 bit machine code. The code will be deciphered and the appropriate instruction will be displayed. Then, the instruction will be executed and the appropriate output will be displayed. First we check whether the instruction is compressed or not. If it is compressed, we expand it to a non compressed instruction and execute it accordingly. The expansion is done following the structure provided by the RISC-V Compressed Instruction Set Manual. For the normal instructions, we decide which format to execute based on the opcode, and then for each format we execute exactly as in the slides. 

Individual Contributions:
Ismaiel: I-type format instructions, B-type instructions, B-type instructions execution, Compressed instructions, Test Cases, ReadMe 
Seif: R-type format instructions, R-type instructions execution, I-type instructions execution, U-type instructions and their execution, J-type instruction and their execution, Compressed Instructions, Testing
Jana: S-type format instructions, Compressed Instructions, Testing
