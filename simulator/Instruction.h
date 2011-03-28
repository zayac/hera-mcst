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
	short int instr_word;
	int cmd_counter;
	vector<int> args;
	string instr_string;
    vector<string>addr_name;
    vector<int> addr_value;
	vector<string> branches;
public:
	Instruction();
	int getInstrWord(){ return instr_word; }
    int getCmdCounter(){ return cmd_counter; }
    void setCmdCounter( int counter) { this->cmd_counter = counter; }
    void incCmdCounter() { this->cmd_counter++; }
	void setInstrWord(int instr) { this->instr_word = instr; }
    void process(string str, unsigned short int* instrs);

    void setValueByShift( int value, int shift)
	{
		this->instr_word = instr_word + (value << shift);
	}

    string toUpper( string str)
    {
        string ret = str;
        for(unsigned int i = 0; i < ret.size(); i++)
        {
            ret[i] = toupper( ret[i]);
        }
        return ret;
    }

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

    int findBranch( string br)
   {
        for ( unsigned int i = 0; i < this->branches.size(); i++)
        {
            if (br == this->branches[i])
                return i;
        }
        return -1;
    }

	int set(string source);
	int encode( unsigned short int* instrs);
	string trim(string str);
	string erase(string str, char chr);
	int setAddr(string str);
};

#endif /* INSTRUCTION_H */
