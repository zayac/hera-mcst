#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <cassert>
#include "ELF_Types.h"
#include <string>
#include <vector>

#include <stdlib.h>
#include <ctype.h>

using namespace std;

// typedef signed short Int16;

class Instruction
{
    /* Instruction encoded in a binary form */
    short int instr_word;                   // A single encoded instruction
    int cmd_counter;                        // Program counter
    vector<int> args;                       // Array of arguments
    string instr_string;                    // Operation string representation, e.g. MOV, SETLO, etc.
    vector<string>addr_name;                // Label names
    vector<int> addr_value;                 // Label values (relative addresses)

    /* just an array of string which are for branch instruction.
     * See constructor to see it's initialization.
     */
    vector<string> branches;                
    public:
    Instruction();

    // just several getters and setters
    int getInstrWord(){ return instr_word; }
    int getCmdCounter(){ return cmd_counter; }
    void setCmdCounter( int counter) { this->cmd_counter = counter; }
    void incCmdCounter() { this->cmd_counter++; }
    void setInstrWord(int instr) { this->instr_word = instr; }

    /* A method that encodes a single instructions.
     * The first argument represents instruction string.
     * The second argument is a pointer to the array of encoded instructions.
     * The third argument is true if the instructions we are to encode has arguments.
     */
    void process(string str, unsigned short int* instrs, bool setArgs);

    /*
     * A very helpful function that sets a given number with a given shift in the target instruction.
     */
    void setValueByShift( int value, int shift)
    {
        this->instr_word = instr_word + (value << shift);
    }

    // The assembler is case-insensitive. That's why we are to uppercase all instructions 
    string toUpper( string str)
    {
        string ret = str;
        for(unsigned int i = 0; i < ret.size(); i++)
        {
            ret[i] = toupper( ret[i]);
        }
        return ret;
    }

    /*
     * Registers in assembler are represented as a number with a prefix,
     * e.g. %5 or r5. In this case just number 5 should be returned.
     */
    int strToInt(string str)
    {
        unsigned int sc = str.find('%');
        if( sc != string::npos)
        {
            str.erase(sc, 1);
        }
        sc = str.find('r');
        if( sc != string::npos)
        {
            str.erase(sc, 1);
        }
        return atoi(str.c_str());
    }

    // Image branch name into it's index in branches vector
    int findBranch( string br)
    {
        for ( unsigned int i = 0; i < this->branches.size(); i++)
        {
            if (br == this->branches[i])
                return i;
        }
        return -1;
    }
    // The function is used to parse source string and to set operation and arguments separately 
    int set(string source, bool setArgs);
    // Encodes instruction to a number. A method set should be called before.
    int encode( unsigned short int* instrs);
    // avoid all useless spaces
    string trim(string str);
    // Remove all chr occurencies in the str
    string erase(string str, char chr);
    // Image label name into the address
    int setAddr(string str);
};

#endif /* INSTRUCTION_H */
