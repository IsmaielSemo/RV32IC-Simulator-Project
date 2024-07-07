/*
    References:
    (1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
    (2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

unsigned int pc;
unsigned char memory[(64 + 64) * 1024]; // 80Kb of memory
unsigned int reg[32]; //the 32 registers
vector<string> Couts;  // output vector
bool exitFlag = false; // if we ecall the exit service (a7 = 10)
signed temp;

void updateReg(int reg_number, int value)                       // not used anymore left for development history/process
{                                                               // function that updates content of register
    if (reg_number == 0)                                        // if the register is the zero register
        cout << "Can’t change the value of register 0" << endl; // don't change it
    else if (reg_number < 0 || reg_number > 31)
        cout << "No such register exists." << endl;
    else
        reg[reg_number] = value; // change it
}

unsigned int readfromReg(unsigned int reg_number) //function that reads value in the registers
{
    if (reg_number >= 31) // since we have 32 registers (0->31)
        cout << "No such register exists." << endl;
    else
        return reg[reg_number];
}

void emitError(const char *s) // for error display
{
    cout << s;
    exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) // displaying the prefix
{
    cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

void instDecExec(unsigned int instWord)
{
    unsigned int rd, rs1, rs2, funct3, funct7, opcode, compressed_opcode, compressed_funct3, compressed_funct2, compressed_funct;
    unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
    unsigned int address;

    unsigned int instPC;

    opcode = instWord & 0x0000007F;         // 00000000000000000000000001111111
    rd = (instWord >> 7) & 0x0000001F;      // 00000000000000000000011111000000
    funct3 = (instWord >> 12) & 0x00000007; // 00000000000000000011100000000000
    funct7 = (instWord >> 25) & 0x0000007F; // 00000000000000000000000001111111
    rs1 = (instWord >> 15) & 0x0000001F;    // 00000000000000011111000000000000
    rs2 = (instWord >> 20) & 0x0000001F;    // 00000000000111110000000000000000

    // — inst[31] — inst[30:25] inst[24:21] inst[20]

    if ((opcode & 0x3) != 0x3) // if the instruction is a compressed one
    {
        instPC = pc - 2; // align with half-words
        I_imm = 0;
        S_imm = 0;
        B_imm = 0;
        U_imm = 0;
        J_imm = 0;
    }
    else
    {
        instPC = pc - 4;                                                                                                                                                                                   // otherwise align with words
        I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));                                                                                                                      // immediate for I-format
        S_imm = ((instWord >> 7) & 0x1F) | (((instWord >> 25) & 0x7F) << 5) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));                                                                                     // Immediate for S format
        B_imm = (((instWord >> 31) & 0x1) << 12) | (((instWord >> 7) & 0x1) << 11) | (((instWord >> 25) & 0x3F) << 5) | (((instWord >> 8) & 0xF) << 1) | 0 | (((instWord >> 31) ? 0xFFFFE000 : 0x0));      // Immediate for B format
        U_imm = ((instWord & 0xFFFFF000) >> 12) | 0;                                                                                                                                                       // Immediate for U format
        J_imm = (((instWord >> 31) & 0x1) << 20) | (((instWord >> 12) & 0xFF) << 12) | (((instWord >> 20) & 0x1) << 11) | (((instWord >> 21) & 0x3FF) << 1) | (((instWord >> 31) & 0x1) ? 0xFFF00000 : 0); // Immediate for J format
    }

    printPrefix(instPC, instWord);

    // Decompression of Compressed Instructions
    if ((opcode & 0x3) != 0x3)
    {
        if (instWord == 0)
        {
            cout << "\tIllegal Instruction\n";
            return;
        }

        compressed_opcode = opcode & 0x3;

        if (compressed_opcode == 0)
        {
            compressed_funct3 = ((instWord >> 13) & 0x7);
            switch (compressed_funct3)
            {
            case 0: // Uncompress to ADDI rd', sp, imm | C.ADDI4SPN
                opcode = 0x13;
                funct3 = 0;
                rd = ((instWord >> 2) & 0x7) + 8; // formula used for computing register (in reference to the manual)
                rs1 = 2;
                rs2 = 0;
                I_imm = ((((unsigned)instWord >> 5) & 0x1) << 3) |
                        ((((unsigned)instWord >> 6) & 0x1) << 2) |
                        ((((unsigned)instWord >> 7) & 0xF) << 6) |
                        ((((unsigned)instWord >> 11) & 0x3) << 4) | (0 << 10);
                break;

            case 2: // uncompress to Lw, rd', imm(rs1') | C.LW
                opcode = 0x3;
                funct3 = 2;
                rd = ((instWord >> 2) & 0x7) + 8;
                rs1 = ((instWord >> 7) & 0x7) + 8;
                I_imm = ((((unsigned)instWord >> 5) & 0x1) << 6) |
                        ((((unsigned)instWord >> 6) & 0x1) << 2) |
                        ((((unsigned)instWord >> 10) & 0x7) << 3) |
                        (0 << 7);
                break;

            case 6: // uncompress to SW, rs2', imm(rs1') | C.SW
                opcode = 0x23;
                funct3 = 2;
                rs2 = ((instWord >> 2) & 0x7) + 8;
                rs1 = ((instWord >> 7) & 0x7) + 8;
                S_imm = ((((unsigned)instWord >> 5) & 0x1) << 6) |
                        ((((unsigned)instWord >> 6) & 0x1) << 2) |
                        ((((unsigned)instWord >> 10) & 0x7) << 3) |
                        (0 << 7);
                break;
            default:
                cout << "\tUnknown C Instruction \n";
                return;
            }
        }
        else if (compressed_opcode == 1)
        {
            compressed_funct3 = (instWord >> 13) & 0x7;
            switch (compressed_funct3)
            {
            case 0: // uncompress to ADDI rd,rd, nzimm[5:0] (Zero = C.NOP) (else = C.ADDI)
                opcode = 0x13;
                funct3 = 0;
                rd = (instWord >> 7) & 0x1F;
                rs1 = rd;
                I_imm = ((instWord >> 2) & 0x1F) |
                        (((instWord >> 12) & 0x1) << 5) |
                        (((instWord >> 12) & 0x1)
                             ? 0xFFFFFFC0
                             : 0);
                break;
            case 1: // uncompress to JAL x1, offset[11:1] | C.JAL
                opcode = 0x6F;
                rd = 1;
                J_imm = (((instWord >> 2) & 0x1) << 5) |
                        (((instWord >> 3) & 0x7) << 1) |
                        (((instWord >> 6) & 0x1) << 7) |
                        (((instWord >> 7) & 0x1) << 6) |
                        (((instWord >> 8) & 0x1) << 10) |
                        (((instWord >> 9) & 0x3) << 8) |
                        (((instWord >> 11) & 0x1) << 4) |
                        (((instWord >> 12) & 0x1) << 11) |
                        (((instWord >> 12) & 0x1) ? 0xFFFFF800 : 0);

                break;
            case 2: // uncompress to LI rd, x0, imm[5:0] | C.LI
                opcode = 0x13;
                funct3 = 0;
                rd = (instWord >> 7) & 0x1F;
                rs1 = 0;
                rs2 = 0;
                I_imm = ((instWord >> 2) & 0x1F) |
                        (((instWord >> 12) & 0x1) << 5) |
                        (((instWord >> 12) & 0x1)
                             ? 0xFFFFFFC0
                             : 0);
                break;
            case 3: // uncompress to LUI rd, imm[17:12] | C.LUI
                rd = (instWord >> 7) & 0x1F;
                if ((rd != 2) && (rd != 0))
                {
                    opcode = 0x37;
                    rd = (instWord >> 7) & 0x1F;
                    U_imm = ((instWord >> 2) & 0x1F) |
                            (((instWord >> 12) & 0x1) << 5) |
                            (((instWord >> 12) & 0x1)
                                 ? 0xFFFFFFC0
                                 : 0);
                }
                else if (rd == 2) // C.ADDI16SP
                {
                    opcode = 0x13;
                    funct3 = 0;
                    rs1 = 2;
                    I_imm = (((instWord >> 2) & 0x1) << 5) |
                            (((instWord >> 3) & 0x3) << 7) |
                            (((instWord >> 5) & 0x1) << 6) |
                            (((instWord >> 6) & 0x1) << 4) |
                            (((instWord >> 12) & 0x1) << 9) |
                            (((instWord >> 12) & 0x1) ? 0xFFFFF800 : 0);
                }

                break;
            case 4:
                compressed_funct2 = ((instWord >> 10) & 0x3);
                switch (compressed_funct2)
                {
                case 0: // uncompress to SRLI rd',rd',shamt
                    opcode = 0x13;
                    funct3 = 5;
                    rd = ((instWord >> 7) & 0x7) + 8;
                    rs1 = rd;
                    I_imm = ((instWord >> 2) & 0x1F) |
                            (((instWord >> 12) & 0x1) << 5);
                    break;
                case 1: // uncompress to SRAI rd',rd',shamt
                    opcode = 0x13;
                    funct3 = 5;
                    rd = ((instWord >> 7) & 0x7) + 8;
                    rs1 = rd;
                    I_imm = ((instWord >> 2) & 0x1F) |
                            (((instWord >> 12) & 0x1) << 5) |
                            0x400;
                    break;
                case 2: // uncompress to ANDI rd',rd',imm
                    opcode = 0x13;
                    funct3 = 0;
                    rd = ((instWord >> 7) & 0x7) + 8;
                    rs1 = rd;
                    I_imm = ((instWord >> 2) & 0x1F) |
                            (((instWord >> 12) & 0x1) << 5);
                    break;
                case 3:
                    compressed_funct = (instWord >> 5) & 0x3;
                    switch (compressed_funct)
                    {
                    case 0: // c.sub format is sub rd', rd', rs2
                        opcode = 0x33;
                        rs2 = ((instWord >> 2) & 0x7) + 8;
                        rd = ((instWord >> 7) & 0x7) + 8;
                        rs1 = rd;
                        funct7 = 32; // to perform subtraction
                        funct3 = 0;
                        break;
                    case 1: // c.xor operation
                        opcode = 0x33;
                        rs2 = ((instWord >> 2) & 0x7) + 8;
                        rd = ((instWord >> 7) & 0x7) + 8;
                        rs1 = rd;
                        funct3 = 4;
                        break;
                    case 2: // c.or operation
                        opcode = 0x33;
                        rs2 = ((instWord >> 2) & 0x7) + 8;
                        rd = ((instWord >> 7) & 0x7) + 8;
                        rs1 = rd;
                        funct3 = 6;
                        break;
                    case 3: // c.and operation
                        opcode = 0x33;
                        rs2 = ((instWord >> 2) & 0x7) + 8;
                        rd = ((instWord >> 7) & 0x7) + 8;
                        rs1 = rd;
                        funct3 = 7;
                        break;
                    }

                    break;
                }
                break;
            case 5: // c.j
                opcode = 0x6F;
                rd = 0;
                J_imm = (((instWord >> 2) & 0x1) << 5) |
                        (((instWord >> 3) & 0x7) << 1) |
                        (((instWord >> 6) & 0x1) << 7) |
                        (((instWord >> 7) & 0x1) << 6) |
                        (((instWord >> 8) & 0x1) << 10) |
                        (((instWord >> 9) & 0x3) << 8) |
                        (((instWord >> 11) & 0x1) << 4) |
                        (((instWord >> 12) & 0x1) << 11) |
                        (((instWord >> 12) & 0x1)
                             ? 0xFFFFF800
                             : 0);
                break;

            case 6: // c.beqz
                opcode = 0x63;
                B_imm = ((((unsigned)instWord >> 2) & 0x1) << 5) |
                        ((((unsigned)instWord >> 3) & 0x3) << 1) |
                        ((((unsigned)instWord >> 5) & 0x3) << 6) |
                        ((((unsigned)instWord >> 10) & 0x3) << 3) |
                        ((((unsigned)instWord >> 12) & 0x3) << 8) |
                        (((instWord >> 13) & 0x1)
                             ? 0xFFFFFF00
                             : 0);
                funct3 = 0;
                rs1 = ((instWord >> 7) & 0x7) + 8;
                rs2 = 0;
                break;
            case 7: // c.bnez
                opcode = 0x63;
                B_imm = ((((unsigned)instWord >> 2) & 0x1) << 5) |
                        ((((unsigned)instWord >> 3) & 0x3) << 1) |
                        ((((unsigned)instWord >> 5) & 0x3) << 6) |
                        ((((unsigned)instWord >> 10) & 0x3) << 3) |
                        ((((unsigned)instWord >> 12) & 0x3) << 8) |
                        (((instWord >> 13) & 0x1)
                             ? 0xFFFFFF00
                             : 0);
                funct3 = 1;
                rs1 = ((instWord >> 7) & 0x7) + 8;
                rs2 = 0;
                break;
            }
        }
        else if (compressed_opcode == 2)
        {
            compressed_funct3 = (instWord >> 13) & 0x7;
            switch (compressed_funct3)
            {
            case 0: // c.slli
                opcode = 0x13;
                funct3 = 1;
                rd = ((instWord >> 7) & 0x7) + 8;
                rs1 = rd;
                I_imm = ((instWord >> 2) & 0x1F) |
                        (((instWord >> 12) & 0x1) << 5);
                break;
            case 2: // c.lwsp
                opcode = 0x3;
                funct3 = 2;
                rd = (instWord >> 7) & 0x1F;
                rs1 = 2;
                I_imm = (((instWord >> 2) & 0x3) << 6) |
                        (((instWord >> 4) & 0x7) << 2) |
                        (((instWord >> 12) & 0x1) << 5);
                break;

            case 4: // c.jr and c.mv
                rs2 = (instWord >> 2) & 0x1F;
                compressed_funct2 = ((instWord >> 12) & 0x1);
                switch (compressed_funct2)
                {
                case 0:
                    if (rs2 == 0)
                    { // c.jr
                        opcode = 0x67;
                        rd = 0;
                        rs1 = (instWord >> 7) & 0x1F;
                        I_imm = 0;
                        break;
                    }
                    else
                    { // c.mv
                        opcode = 0x33;
                        funct3 = 0;
                        funct7 = 0;
                        rd = (instWord >> 7) & 0x1F;
                        rs1 = 0;
                        break;
                    }
                    break;
                case 1:
                    if (rs2 == 0)
                    { // c.jalr
                        opcode = 0x67;
                        rd = 1;
                        rs1 = (instWord >> 7) & 0x1F;
                        I_imm = 0;
                        break;
                    }
                    else
                    { // c.add
                        opcode = 0x33;
                        funct3 = 0;
                        funct7 = 0;
                        rd = (instWord >> 7) & 0x1F;
                        rs1 = rd;
                        break;
                    }

                    break;
                }

                break;

            case 6: // c.swsp
                opcode = 0x23;
                funct3 = 2;
                rs2 = (instWord >> 2) & 0x1F;
                rs1 = 2;
                S_imm = (((instWord >> 7) & 0x3) << 6) |
                        (((instWord >> 9) & 0xF) << 2);

                break;
            }
        }
        else
        {
            cout << "Unknown C instruction" << endl;
        }
    }

    // Everything below are uncompressed instructions
    if (opcode == 0x33)
    { // R Instructions
        switch (funct3)
        {
        case 0:
            if (funct7 == 32) //sub operation
            {
                cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                reg[rd] = (signed)reg[rs1] - (signed)reg[rs2]; //the execution of the operation
            }
            else //add operation
            {
                cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                reg[rd] = (signed)reg[rs1] + (signed)reg[rs2]; //the execution of the operation
            }
            break;
        case 1: //shift logical left operation
            cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = reg[rs1] << reg[rs2]; //execution of operation
            break;
        case 2:
            cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //SLT operation
            reg[rd] = ((signed)reg[rs1] < (signed)reg[rs2]) ? 1 : 0; //execution of operation
            break;
        case 3:
            cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //SLTU operation
            reg[rd] = (reg[rs1] < reg[rs2]) ? 1 : 0;//execution of operation
            break;
        case 4:
            cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //XOR operation
            reg[rd] = reg[rs1] ^ reg[rs2];//execution of operation
            break;
        case 5:
            if (funct7 == 32)
            {
                cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //SRA operation
                reg[rd] = (signed)reg[rs1] >> reg[rs2]; //execution of operation
                if ((signed)reg[rs1] < 0)
                    reg[rd] = (reg[rd] | 0xFFFFFFFF) << (32 - reg[rs2]);
            }
            else
            {
                cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //SRL operation
                reg[rd] = reg[rs1] >> reg[rs2]; //execution of operation
            }
            break;
        case 6:
            cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //OR operation
            reg[rd] = reg[rs1] | reg[rs2]; //execution of operation
            break;
        case 7:
            cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; //AND operation
            reg[rd] = reg[rs1] & reg[rs2]; //execution of operation
            break;
        default:
            cout << "\tUnknown R Instruction \n";
        }
    }
    else if (opcode == 0x13)
    { // I-type instructions (everything except load)
        switch (funct3)
        {
        case 0:
            cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //Addi operation
            reg[rd] = (signed)reg[rs1] + (signed)I_imm; //execution of operation
            break;
        case 2:
            cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //SLTI operation
            reg[rd] = ((signed)reg[rs1] < (signed)I_imm) ? 1 : 0; //execution of operation
            break;
        case 3:
            cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //SLTIU operation
            reg[rd] = (reg[rs1] < I_imm) ? 1 : 0; //execution of operation
            break;
        case 4:
            cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //XORI operation
            reg[rd] = reg[rs1] ^ I_imm; //execution of operation
            break;
        case 6:
            cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //ORI operation
            reg[rd] = reg[rs1] | I_imm; //execution of operation
            break;
        case 7:
            cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //ANDI operation
            reg[rd] = reg[rs1] & I_imm; //execution of operation
            break;
        case 1:
            cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << ((int)I_imm & 31) << "\n"; //SLLI operation
            reg[rd] = reg[rs1] << (I_imm & 31); //execution of operation
            break;
        case 5:
            if ((I_imm & 0x400) == 1024)
            {
                cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << ((int)I_imm & 31) << "\n"; //SRAI operation
                reg[rd] = (signed)reg[rs1] >> (I_imm & 31); //execution of operation
                if ((signed)reg[rs1] < 0)
                    reg[rd] = (reg[rd] | 0xFFFFFFFF) << (32 - (I_imm & 31));
            }
            else
            {
                cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << ((int)I_imm & 31) << "\n"; //SRLI operation
                reg[rd] = reg[rs1] >> (I_imm & 31); //execution of operation
            }
            break;
        default:
            cout << "\tUnknown I Instruction \n";
        }
    }
    else if (opcode == 0x3)
    { // I-type instructions (load instructions)
        switch (funct3)
        {
        case 0:
            cout << "\tLB\tx" << rd << ", " << (int)I_imm << "(x" << rs1 << ")\n";
            reg[rd] = (memory[(signed)reg[rs1] + (signed)I_imm]) | (((memory[(signed)reg[rs1] + (signed)I_imm] & 0x80) == 0x80) ? 0xFFFFFF00 : 0);
            break;
        case 1:
            cout << "\tLH\tx" << rd << ", " << (int)I_imm << "(x" << rs1 << ")\n";
            reg[rd] = (memory[(signed)reg[rs1] + (signed)I_imm]) | (memory[(signed)reg[rs1] + (signed)I_imm + 1] << 8) | ((((signed)memory[reg[rs1] + (signed)I_imm + 1] & 0x80) == 0x80) ? 0xFFFF0000 : 0);
            break;
        case 2:
            cout << "\tLW\tx" << rd << ", " << (int)I_imm << "(x" << rs1 << ")\n";
            // cout << (signed)reg[rs1] + (signed)I_imm << (signed)reg[rs1] << endl;
            reg[rd] = (memory[(signed)reg[rs1] + (signed)I_imm]) | (memory[(signed)reg[rs1] + (signed)I_imm + 1] << 8) | ((signed)memory[reg[rs1] + (signed)I_imm + 2] << 16) | ((signed)memory[reg[rs1] + (signed)I_imm + 3] << 24);
            break;
        case 4:
            cout << "\tLBU\tx" << rd << ", " << (int)I_imm << "(x" << rs1 << ")\n";
            reg[rd] = memory[(signed)reg[rs1] + (signed)I_imm];
            break;
        case 5:
            cout << "\tLHU\tx" << rd << ", " << (int)I_imm << "(x" << rs1 << ")\n";
            reg[rd] = memory[(signed)reg[rs1] + (signed)I_imm] | (memory[(signed)reg[rs1] + (signed)I_imm + 1] << 8);
            break;
        default:
            cout << "\tUnknown I Instruction \n";
        }
    }
    else if (opcode == 0x23) // S-type instructions
    {
        switch (funct3)
        {
        case 2: // sw (store word)
            cout << "\tSW\tx" << rs2 << ", " << (signed)S_imm << "(" << "x" << rs1 << ")\n";
            memory[(signed)((signed)reg[rs1] + (signed)S_imm)] = (reg[rs2] & 0xFF);
            memory[(signed)((signed)reg[rs1] + (signed)S_imm + 1)] = ((reg[rs2] >> 8) & 0xFF);
            memory[(signed)((signed)reg[rs1] + (signed)S_imm + 2)] = ((reg[rs2] >> 16) & 0xFF);
            memory[(signed)((signed)reg[rs1] + (signed)S_imm + 3)] = ((reg[rs2] >> 24) & 0xFF);
            break;
        case 1: // sh (store half word)
            cout << "\tSH\tx" << rs2 << ", " << (signed)S_imm << "(" << "x" << rs1 << ")\n";
            memory[(signed)((signed)reg[rs1] + (signed)S_imm)] = (reg[rs2] & 0xFF);
            memory[(signed)((signed)reg[rs1] + (signed)S_imm + 1)] = ((reg[rs2] >> 8) & 0xFF);
            break;
        case 0: // sb (store byte)
            cout << "\tSB\tx" << rs2 << ", " << (signed)S_imm << "(" << "x" << rs1 << ")\n";
            memory[(signed)((signed)reg[rs1] + (signed)S_imm)] = (reg[rs2] & 0xFF);
            break;
        default:
            cout << "\tUnknown S type instruction\n";
        }
    }
    else if (opcode == 0x63) // B-type instructions
    {
        switch (funct3)
        {
        case 0:
            cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << ", " << (int)B_imm << "\n"; //BEQ operation
            if (reg[rs1] == reg[rs2])
                pc = (signed)B_imm + instPC; //execution of operation
            break;
        case 1:
            cout << "\tBNE\tx" << rs1 << ", x" << rs2 << ", " << (int)B_imm << "\n"; //BNE operation
            if (reg[rs1] != reg[rs2])
                pc = (signed)B_imm + instPC; //execution of operation
            break;
        case 4:
            cout << "\tBLT\tx" << rs1 << ", x" << rs2 << ", " << (int)B_imm << "\n"; //BLT operation
            if ((signed)reg[rs1] < (signed)reg[rs2]) //execution of operation
                pc = (signed)B_imm + instPC;
            break;
        case 5:
            cout << "\tBGE\tx" << rs1 << ", x" << rs2 << ", " << (int)B_imm << "\n"; //BGE operation
            if ((signed)reg[rs1] >= (signed)reg[rs2]) //execution of operation
                pc = (signed)B_imm + instPC;
            break;
        case 6:
            cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << ", " << (int)B_imm << "\n"; //BLTU operation
            if (reg[rs1] < reg[rs2]) //execution of operation
                pc = (signed)B_imm + instPC;
            break;
        case 7:
            cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << ", " << (int)B_imm << "\n"; //BGEU operation
            if (reg[rs1] >= reg[rs2]) //execution of operation
                pc = (signed)B_imm + instPC;
            break;
        default:
            cout << "\tUnknown B Instruction \n";
        }
    }
    else if (opcode == 0x37) // U-type instructions
    {
        cout << "\tLUI\tx" << rd << ", " << hex << "0x" << (int)U_imm << "\n"; //LUI operation
        reg[rd] = ((int)U_imm << 12); //execution of operation
    }
    else if (opcode == 0x17) // U-type instructions
    {
        cout << "\tAUIPC\tx" << rd << ", " << hex << "0x" << (int)U_imm << "\n";// AUIPC operation
        reg[rd] = ((int)U_imm << 12) + instPC; //execution of operation
    }
    else if (opcode == 0x6F) // J-type instructions
    {
        cout << "\tJAL\tx" << rd << ", " << hex << "0x" << (int)J_imm << "\n"; //JAL operation
        reg[rd] = pc; //execution of operation and the line below
        pc = (J_imm) + instPC;
    }
    else if (opcode == 0x67) // I-type instructions
    {
        cout << "\tJALR\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n"; //JALR oepration
        reg[rd] = pc; //execution of operation and the line below
        pc = (reg[rs1] + (signed)I_imm) & 0xFFFFFFFE;
    }
    else if (opcode == 0x73) // E-call instructions
    {
        if (funct3 == 0)
        {
            if (rd == 0 && rs1 == 0 && rs2 == 0)
            {
                cout << "\tECALL\n";
                switch (reg[17])
                {
                case 1: //ECALL SERVICE 1
                    Couts.push_back(to_string((signed)reg[10]) + "\n"); //execution of operation (for ecall 1)
                    break;
                case 4:
                    Couts.push_back(string(1, memory[reg[10] /* - (0xfc00 << 12)*/]));
                    address = reg[10] + 1 /* - (0xfc00 << 12)*/;
                    while (memory[address] != 0)
                    {
                        Couts[Couts.size() - 1] += memory[address];
                        address++;
                    }
                    break;
                case 10: //execution of operation (for ecall 10)
                    Couts.push_back("Program exited with code " + to_string(reg[10]) + "\n");
                    exitFlag = true;
                    break;
                default:
                    cout << "\tUnknown Ecall\n";
                    break;
                }
            }
            else
            {
                cout << "\tEBREAK\n";
            }
        }
        else
        {
            cout << "\tUnknown Instruction \n";
        }
    }
    else
    {
        cout << "\tUnknown Instruction \n";
    }
}

int main(int argc, char *argv[])
{
    reg[0] = 0;             // initialize the zero register
    reg[2] = 64 * 1024 - 1; // initialize the stack pointer
    unsigned int instWord = 0;
    ifstream inFile;
    ifstream inFile2;
    ofstream outFile;

    if (argc < 1)
        emitError("use: rvcdiss <machine_code_file_name>\n");

    inFile.open(argv[1], ios::in | ios::binary | ios::ate);
    inFile2.open(argv[2], ios::in | ios::binary | ios::ate);

    if (inFile.is_open() && inFile2.is_open())
    {
        int fsize = inFile.tellg();

        inFile.seekg(0, inFile.beg);
        if (!inFile.read((char *)memory, fsize))
            emitError("Cannot read from input file\n");

        int fsize2 = inFile2.tellg();

        inFile2.seekg(0, inFile2.beg);
        if (!inFile2.read((char *)(memory + 64 * 1024), fsize2))
            emitError("Cannot read from input file\n");

        int i = 0;

        while (true)
        {
            if (((unsigned char)memory[pc] & 0x3) != 0x3)
            {
                instWord = (unsigned char)memory[pc] |
                           (((unsigned char)memory[pc + 1]) << 8) |
                           0x0000;
                pc += 2;
            }
            else
            {
                instWord = (unsigned char)memory[pc] |
                           (((unsigned char)memory[pc + 1]) << 8) |
                           (((unsigned char)memory[pc + 2]) << 16) |
                           (((unsigned char)memory[pc + 3]) << 24);
                pc += 4;
            }
            i++;
            if (i == 65536)
            {
                break;
                cout << "\nProgram too long" << endl;
            }
            reg[0] = 0;
            instDecExec(instWord);
            if (exitFlag)
                break;
        }

        cout << "\nOutput: " << endl; //display
        for (int i = 0; i < Couts.size(); i++)
        {
            cout << Couts[i];
        }
        cout << "\nRegisters===================================" << endl;
        for (int i = 0; i < 32; i++)
        {
            cout << "x" << dec << i << " = " << hex << "0x" << reg[i] << endl;
        }

        inFile.close();
        inFile2.close();
    }
    else
        emitError("Cannot access input file\n");

    return 0;
}
