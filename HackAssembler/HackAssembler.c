//assembler_modified5.c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

int main(int argc, char* argv[])
{

    //Initialization
    //Opens the input file, and gets ready to process it
    const char* assemblyFilename;
    if (argc < 2) {
        fprintf(stderr, "Assembly filename is ../file.asm\n");
        assemblyFilename = "../file.asm";
    }
    else
        assemblyFilename = argv[1];
    if (!parser_init(assemblyFilename)) {
        fprintf(stderr, "Usage: assembler filename (default ../file.asm)\n");
        perror("fopen");
        exit(0);
    }
    //adds it all predefined symbols
    symbolTable_addEntry("R0", 0);
    symbolTable_addEntry("R1", 1);
    symbolTable_addEntry("R2", 2);
    symbolTable_addEntry("R3", 3);
    symbolTable_addEntry("R4", 4);
    symbolTable_addEntry("R5", 5);
    symbolTable_addEntry("R6", 6);
    symbolTable_addEntry("R7", 7);
    symbolTable_addEntry("R8", 8);
    symbolTable_addEntry("R9", 9);
    symbolTable_addEntry("R10", 10);
    symbolTable_addEntry("R11", 11);
    symbolTable_addEntry("R12", 12);
    symbolTable_addEntry("R13", 13);
    symbolTable_addEntry("R14", 14);
    symbolTable_addEntry("R15", 15);
    symbolTable_addEntry("SCREEN", 16384);
    symbolTable_addEntry("KBD", 24576);
    symbolTable_addEntry("SP", 0);
    symbolTable_addEntry("LCL", 1);
    symbolTable_addEntry("ARG", 2);
    symbolTable_addEntry("THIS", 3);
    symbolTable_addEntry("THAT", 4);

    //first pass:build the symbol table for labels ,scans the file to find label definitions
    char line[LINE_MAX_SIZE];
    char symbol[LINE_MAX_SIZE];
    int instructionCounter = 0;
    while (parser_parseNextLine(line))
    {
        InstructionTypeEnum type = parser_instructionType();
        if (type == L_INSTRUCTION)
        {
            parser_symbol(symbol);
            if (symbolTable_getAddress(symbol)==-1)
            {
                symbolTable_addEntry(symbol, instructionCounter);
            }
        }
        else if (type == A_INSTRUCTION || type == C_INSTRUCTION)
        {
            instructionCounter++;
        }
    }
    //reset parser o the beginning of the file for the second pass 
    parser_rewind();
    //prepar the output.hack file 
    char hackFilename[100];
    strcpy(hackFilename, assemblyFilename);
    char* dotPosition = strrchr(hackFilename, '.');
    if (dotPosition != NULL) {
        strcpy(dotPosition + 1, "hack"); 
    }
    FILE* hackFP = fopen(hackFilename, "wt");
    if (hackFP == NULL) { exit(1); }

    //second pass,translate each instruction into it's 16 bit binary representation 
    char addressStr[LINE_MAX_SIZE];
    char dest[LINE_MAX_SIZE];
    char comp[LINE_MAX_SIZE];
    char jump[LINE_MAX_SIZE];
    int variableAddress = 16;//variable start at RAM adress 16  
    char binaryLine[18];
    int addressValue;

    while (parser_parseNextLine(line))
    {
        InstructionTypeEnum type = parser_instructionType();
        if (type == COMMENT_OR_EMPTY || type == L_INSTRUCTION)
        {
            continue;//skip lines that don't generate machine code 
        }
        else if (type == A_INSTRUCTION) {
            parser_address(addressStr);
            if (isdigit(addressStr[0])) {//constant adress
                addressValue = atoi(addressStr);
            }
            else {
                addressValue = symbolTable_getAddress(addressStr);//symbolic adress
                if (addressValue == -1) {
                    //new variable 
                    
                    symbolTable_addEntry(addressStr, variableAddress);
                    addressValue = variableAddress++;
                }
            }
            // Convert to 16-bit binary string and write to file.
            code_convertIntToBinString(addressValue, binaryLine);
            fprintf(hackFP, "%s\n", binaryLine);
        }

        else if (parser_instructionType() == C_INSTRUCTION)
        {
            char destBin[4], compBin[8], jumpBin[4];

         // Parse C-instruction components: dest=comp;jump
        parser_dest(dest);
        parser_comp(comp);
        parser_jump(jump);

        // Convert components to binary bits.
        code_dest(dest, destBin);
        code_comp(comp, compBin);
        code_jump(jump, jumpBin);
        // Format: 111 [comp] [dest] [jump]
        fprintf(hackFP, "111%s%s%s\n", compBin, destBin, jumpBin);
        }



    }
    fclose(hackFP);
}


        



        




    
