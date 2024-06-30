/*
    This is just a skeleton. It DOES NOT implement all the requirements.
    It only recognizes the RV32I "ADD", "SUB" and "ADDI" instructions only.
    It prints "Unkown Instruction" for all other instructions!

    References:
    (1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
    (2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

unsigned int pc;
unsigned char memory[(64 + 64) * 1024]; // 80Kb of memory (might be changed to 128Kb)
unsigned int reg[32];

void updateReg(int reg_number, int value)
{                                                               // function that updates content of register
    if (reg_number == 0)                                        // if the register is the zero register
        cout << "Can’t change the value of register 0" << endl; // don't change it
    else if (reg_number < 0 || reg_number > 31)
        cout << "No such register exists." << endl;
    else
        reg[reg_number] = value; // change it
}

unsigned int readfromReg(unsigned int reg_number)
{
    if (reg_number >= 31)
        cout << "No such register exists." << endl;
    else
        return reg[reg_number];
}

void emitError(char *s)
{
    cout << s;
    exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW)
{
    cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

void instDecExec(unsigned int instWord)
{
    unsigned int rd, rs1, rs2, funct3, funct7, opcode;
    unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
    unsigned int address;

    unsigned int instPC = pc - 4;

    opcode = instWord & 0x0000007F;         // 00000000000000000000000001111111
    rd = (instWord >> 7) & 0x0000001F;      // 00000000000000000000011111000000
    funct3 = (instWord >> 12) & 0x00000007; // 00000000000000000011100000000000
    rs1 = (instWord >> 15) & 0x0000001F;    // 00000000000000011111000000000000
    rs2 = (instWord >> 20) & 0x0000001F;    // 00000000000111110000000000000000

    // — inst[31] — inst[30:25] inst[24:21] inst[20]
    I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));
    S_imm = ((instWord >> 7) & 0x1F) | ((instWord >> 25) & 0x7F) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));
    B_imm = (((instWord >> 31) & 0x1) << 12) | (((instWord >> 7) & 0x1) << 11) | (((instWord >> 25) & 0x3F) << 5) | (((instWord >> 8) & 0xF) << 1) | 0 | (((instWord >> 31) ? 0xFFFFE000 : 0x0));
    U_imm = (instWord & 0xFFFFF000);
    J_imm = (((instWord >> 31) & 0x1) << 20) | (((instWord >> 12) & 0xFF) << 12) | (((instWord >> 20) & 0x1) << 11) | (((instWord >> 21) & 0x3FF) << 1) | 0;

    printPrefix(instPC, instWord);

    if (opcode == 0x33)
    { // R Instructions
        switch (funct3)
        {
        case 0:
            if (funct7 == 32)
            {
                cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                reg[rd] = (signed)reg[rs1] - (signed)reg[rs2];
            }
            else
            {
                cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                reg[rd] = (signed)reg[rs1] + (signed)reg[rs2];
            }
            break;
        case 1:
            cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = reg[rs1] << reg[rs2];
            break;
        case 2:
            cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = ((signed)reg[rs1] < (signed)reg[rs2]) ? 1 : 0;
            break;
        case 3:
            cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = (reg[rs1] < reg[rs2]) ? 1 : 0;
            break;
        case 4:
            cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = reg[rs1] ^ reg[rs2];
            break;
        case 5:
            if (funct7 == 32)
            {
                cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                reg[rd] = (signed)reg[rs1] >> reg[rs2];
            }
            else
            {
                cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                reg[rd] = reg[rs1] >> reg[rs2];
            }
            break;
        case 6:
            cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = reg[rs1] | reg[rs2];
            break;
        case 7:
            cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
            reg[rd] = reg[rs1] & reg[rs2];
            break;
        default:
            cout << "\tUnkown R Instruction \n";
        }
    }
    else if (opcode == 0x13)
    { // I-type instructions (everything except load)
        switch (funct3)
        {
        case 0:
            cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = (signed)reg[rs1] + (signed)I_imm;
            break;
        case 2:
            cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = ((signed)reg[rs1] < (signed)I_imm) ? 1 : 0;
            break;
        case 3:
            cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = (reg[rs1] < I_imm) ? 1 : 0;
            break;
        case 4:
            cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = reg[rs1] ^ I_imm;
            break;
        case 6:
            cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = reg[rs1] | I_imm;
            break;
        case 7:
            cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = reg[rs1] & I_imm;
            break;
        case 1:
            cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << ((int)I_imm & 31) << "\n";
            reg[rd] = reg[rs1] << (I_imm & 31);
            break;
        case 5:
            if (I_imm & 0x400 == 1024)
            {
                cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << ((int)I_imm & 31) << "\n";
                reg[rd] = (signed)reg[rs1] >> (I_imm & 31);
            }
            else
            {
                cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << ((int)I_imm & 31) << "\n";
                reg[rd] = reg[rs1] >> (I_imm & 31);
            }
            break;
        default:
            cout << "\tUnkown I Instruction \n";
        }
    }
    else if (opcode == 0x3)
    { // I-type instructions (load instructions)
        switch (funct3)
        {
        case 0:
            cout << "\tLB\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = (memory[reg[rs1] + (signed)I_imm]) | (((memory[reg[rs1] + (signed)I_imm] & 0x80) == 0x80) ? 0xFFFFFF00 : 0);
            break;
        case 1:
            cout << "\tLH\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = (memory[reg[rs1] + (signed)I_imm]) | (memory[reg[rs1] + (signed)I_imm + 1] << 8) | (((memory[reg[rs1] + (signed)I_imm + 1] & 0x80) == 0x80) ? 0xFFFF0000 : 0);
            break;
        case 2:
            cout << "\tLW\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = (memory[reg[rs1] + (signed)I_imm]) | (memory[reg[rs1] + (signed)I_imm + 1] << 8) | (memory[reg[rs1] + (signed)I_imm + 2] << 16) | (memory[reg[rs1] + (signed)I_imm + 3] << 24);
            break;
        case 4:
            cout << "\tLBU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = memory[reg[rs1] + (signed)I_imm];
            break;
        case 5:
            cout << "\tLHU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
            reg[rd] = memory[reg[rs1] + (signed)I_imm] | (memory[reg[rs1] + (signed)I_imm + 1] << 8);
            break;
        default:
            cout << "\tUnkown I Instruction \n";
        }
    }
    else if (opcode == 0x23) // S-type instructions
    {
        switch (funct3) {
            case 0: // sw (store word)
                cout << "\tSW\tx" << rs2 << (funct7 + rd) << "(" << rs1 << ")\n";
		    
		reg[rs2] & 0xFF =  memory[reg[rs1] + S_imm]; //storing first 8 bits
		(reg[rs2] >> 8) & 0xFF =  memory[reg[rs1] + S_imm+1]; //storing the next 8 bits
	        (reg[rs2] >> 16) & 0xFF = memory[reg[rs1] + S_imm + 2]; //storing the next 8 bits
	        (reg[rs2] >> 24) & 0xFF = memory[reg[rs1] + S_imm + 3]; //storing the next 8 bits
                break;
	    case 1: //sh (store half word)
		cout <<  "\tSH\tx" << rs2 << (funct7 + rd) << "(" << rs1 << ")\n";
		    
	        reg[rs2] & 0xFF = memory[reg[rs1] + S_imm] ; // storing the first 8 bits
                (reg[rs2] >> 8) & 0xFF = memory[reg[rs1] + S_imm + 1] ; // storing the next 8 bits
		break;
	    case 2: //sb (store byte)
		cout <<  "\tSB\tx" << rs2 << (funct7 + rd) << "(" << rs1 << ")\n";
		    
		 reg[rs2] & 0xFF = memory[reg[rs1] + S_imm] ; 
		break;
		default:
		cout<<"\tunknown S type instruction\n";
        }
    }
    else if (opcode == 0x63) // B-type instructions
    {
        switch (funct3)
        {
        case 0:
            cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
            break;
        case 1:
            cout << "\tBNE\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
            break;
        case 4:
            cout << "\tLBLT\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
            break;
        case 5:
            cout << "\tBGE\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
            break;
        case 6:
            cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
            break;
        case 7:
            cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
            break;
        default:
            cout << "\tUnkown I Instruction \n";
        }
    }
    else if (opcode == 0x37) // U-type instructions
    {
        cout << "\tLUI\tx" << rd << ", " << hex << "0x" << (int)U_imm << "\n";
    }
    else if (opcode == 0x17) // U-type instructions
    {
        cout << "\tAUIPC\tx" << rd << ", " << hex << "0x" << (int)U_imm << "\n";
    }
    else if (opcode == 0x6F) // J-type instructions
    {
        cout << "\tJAL\tx" << rd << ", " << hex << "0x" << (int)J_imm << "\n";
    }
    else if (opcode == 0x67) // J-type instructions
    {
        cout << "\tJALR\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
    }
    else if (opcode == 0x73) // E-call instructions
    {
        if (funct3 == 0)
        {
            if (rd == 0 && rs1 == 0 && rs2 == 0)
            {
                cout << "\tECALL\n";
            }
            else
            {
                cout << "\tEBREAK\n";
            }
        }
        else
        {
            cout << "\tUnkown Instruction \n";
        }
    }
    else
    {
        cout << "\tUnkown Instruction \n";
    }
}

int main(int argc, char *argv[])
{
    reg[0] = 0; // initialize the zero register
    unsigned int instWord = 0;
    ifstream inFile;
    ofstream outFile;

    if (argc < 1)
        emitError("use: rvcdiss <machine_code_file_name>\n");

    inFile.open(argv[1], ios::in | ios::binary | ios::ate);

    if (inFile.is_open())
    {
        int fsize = inFile.tellg();

        inFile.seekg(0, inFile.beg);
        if (!inFile.read((char *)memory, fsize))
            emitError("Cannot read from input file\n");

        while (true)
        {
            instWord = (unsigned char)memory[pc] |
                       (((unsigned char)memory[pc + 1]) << 8) |
                       (((unsigned char)memory[pc + 2]) << 16) |
                       (((unsigned char)memory[pc + 3]) << 24);
            pc += 4;
            instDecExec(instWord);
        }
    }
    else
        emitError("Cannot access input file\n");
}
