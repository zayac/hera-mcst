#include <iostream>
#include <stdio.h>
#include <string>
#include "Instruction.h"
#include <string.h>

#define if_cmd(cmd) if (!strcmp(instr_string.c_str(), cmd))

using namespace std;
Instruction::Instruction()
{
	this->instr_word = 0;
    this->cmd_counter = 1;
    string branches[] = { "BR", "BL",  "BGE",  "BLE",  "BG",  "BULE",  "BUG",  "BZ",  "BNZ",  "BC",  "BNC",  "BS",  "BNS",  "BV",  "BNV",
                         "BRR", "BLR", "BGER", "BLER", "BGR", "BULER", "BUGR", "BZR", "BNZR", "BCR", "BNCR", "BSR", "BNSR", "BVR", "BNVR" };
    this->branches.insert(this->branches.begin(), branches, branches + 30);
}


string Instruction::trim(string str)
{

	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
	return str;
}

string Instruction::erase(string str, char chr)
{
    string::size_type pos = str.find_first_of(chr);
    while( pos != string::npos)
    {
        str.erase(pos, 1);
        pos = str.find_first_of(chr);
    }
    return str;
}

int Instruction::setAddr(string str)
{
    for(unsigned int i = 0; i < addr_name.size(); i++)
    {
        if(str == addr_name[i])
        {
            return addr_value[i];
        }
    }
    return -1;
}

int Instruction::set( string source, bool setArgs)
{
    this->instr_word = 0;
    this->args.clear();
    string token;
    size_t p0 = 0, p1 = string::npos;
    source = erase(source, '\t');          // avoid all tabulations
    
    /* This is a label check.
     * A character that can be met only in labels is ':'
     * We are to transform label name into relative address
     * and to store for future needs.
     */
    p1 = source.find_first_of(":", p0);

    if (p1 != string::npos)
    {
        addr_name.push_back(source.substr(p0, p1 - p0));
        addr_value.push_back( this->cmd_counter);
        return 0;
    }
    else
    {
        /*
         * This is a comment check.
         */
        p0 = 0;
        size_t p2 = source.find_first_of("#", p0);
        p1 = source.size();
        source = trim(source.substr(0, p2));

        /* 
         * Find an operation name, e.g MOV, SETLO, etc.
         */
        p1 = source.find_first_of(" ", p0);
        if (p1 != p0)
        {
            token = source.substr(p0, p1 - p0);
            this->instr_string = trim(toUpper(token));
        }

        /*
         * Process instruction arguments
         */
        if (setArgs)
        {
            p0 = source.find_first_of(" ", p0);
            while(p0 != string::npos)
            {
                p1 = source.find_first_of(",", p0);
                if(p1 != p0)
                {
                    token = source.substr(p0, p1 - p0);
                    if (setAddr(token) == -1)
                        this->args.push_back(strToInt(trim(token)));
                    else
                        this->args.push_back(setAddr(token));
                }
                p0 = source.find_first_not_of(",", p1);
            }
        }
        return 1;
    }
}

void Instruction::process (string str, unsigned short int* instrs, bool setArgs)
{
    if (this->set(str, setArgs))
    {
        if( this->encode(instrs))
        {
            instrs[this->getCmdCounter()] = this->getInstrWord();
            cout << cmd_counter << ") " << str << " -> " << ( unsigned short int) instr_word << endl;
            this->incCmdCounter();
        }
    }
}

/*
 * Switch can't be used for strings. That's why we use macros here.
 * May be not the best solution
 */
int Instruction::encode(unsigned short int* instrs)
{
    // SETLO and SETHI
	if_cmd("SETLO")
	{
		setValueByShift( 0xE, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 0);
		return 1;
	}
	else if_cmd("SETHI")
	{
		setValueByShift( 0xF, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 0);
		return 1;
	}
	// Tree-address operations
	else if_cmd("AND")
	{
		setValueByShift( 0x1, 15);
		setValueByShift( 0x0, 12);
		setValueByShift( this->args[1], 8);
		setValueByShift( this->args[2], 4);
		setValueByShift( this->args[3], 0);
		return 1;
	}
	else if_cmd("OR")
	{
		setValueByShift( 0x1, 15);
		setValueByShift( 0x1, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 4);
		setValueByShift( this->args[2], 0);
		return 1;
	}
	else if_cmd("ADD")
	{
		setValueByShift( 0x1, 15);
		setValueByShift( 0x2, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 4);
		setValueByShift( this->args[2], 0);
		return 1;
	}
	else if_cmd("SUB")
	{
		setValueByShift( 0x1, 15);
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 4);
		setValueByShift( this->args[2], 0);
                return 1;
	}
	else if_cmd("MULT")
	{
		setValueByShift( 0x1, 15);
		setValueByShift( 0x4, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 4);
		setValueByShift( this->args[2], 0);
                return 1;
	}
	else if_cmd("XOR")
	{
		setValueByShift( 0x1, 15);
		setValueByShift( 0x5, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( this->args[1], 4);
		setValueByShift( this->args[2], 0);
                return 1;
	}
	// Shifts
	else if_cmd("LSL")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( 0x0, 4);
		setValueByShift( this->args[1], 0);
                return 1;
	}
	else if_cmd("LSR")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( 0x1, 4);
		setValueByShift( this->args[1], 0);
                return 1;
	}
	else if_cmd("LSL8")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( 0x2, 4);
		setValueByShift( this->args[1], 0);
                return 1;
	}
	else if_cmd("LSR8")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( 0x3, 4);
		setValueByShift( this->args[1], 0);
                return 1;
	}
	else if_cmd("ASL")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( 0x4, 4);
		setValueByShift( this->args[1], 0);
                return 1;
	}
	else if_cmd("ASR")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( this->args[0], 8);
		setValueByShift( 0x5, 4);
		setValueByShift( this->args[1], 0);
                return 1;
	}
	// Set/Clear flags
    else if_cmd("SETF")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( 0x0, 11);
		setValueByShift( (0x10 & this->args[0]) >> 4, 8);
		setValueByShift( 0xF & this->args[0], 0);
		setValueByShift( 0x3, 5);
                return 1;
	}
    else if_cmd("CLRF")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( 0x1, 11);
		setValueByShift( (0x10 & this->args[0]) >> 4, 8);
		setValueByShift( 0xF & this->args[0], 0);
		setValueByShift( 0x3, 5);
                return 1;
	}
	// Save/restore flags
    else if_cmd("SAVEF")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( 0x7, 4);
		setValueByShift( 0x0, 3);
		setValueByShift( this->args[0], 8);
                return 1;
	}
    else if_cmd("RSTRF")
	{
		setValueByShift( 0x3, 12);
		setValueByShift( 0x7, 4);
		setValueByShift( 0x1, 3);
                setValueByShift( this->args[0], 8);
                return 1;
    }
	// Increments
    else if_cmd("INC")
	{
        setValueByShift( 0x3, 12);
        setValueByShift( 0x2, 6);
        setValueByShift( this->args[0], 8);
        setValueByShift( this->args[1], 0);
        return 1;
	}
    else if_cmd("DEC")
	{
        setValueByShift( 0x3, 12);
        setValueByShift( 0x3, 6);
        setValueByShift( this->args[0], 8);
        setValueByShift( this->args[1], 0);
        return 1;
	}
	// Memory Instructions
    else if_cmd("LOAD")
	{
        setValueByShift( 0x2, 13);
        setValueByShift( this->args[0], 8);
		setValueByShift( (0x10 & this->args[1]) >> 4, 12);
		setValueByShift(  0xF & this->args[1], 4);
		setValueByShift( this->args[2], 0);
        return 1;
	}
    else if_cmd("STORE")
	{
        setValueByShift( 0x3, 13);
        setValueByShift( this->args[0], 8);
		setValueByShift( (0x10 & this->args[1]) >> 4, 12);
		setValueByShift(  0xF & this->args[1], 4);
		setValueByShift( this->args[2], 0);
        return 1;
	}
	// Branches, including jumps
    else if (this->instr_string[0] == 'B')
    {
        int brnum = findBranch( instr_string);
        if (brnum != -1)
        {
            if( brnum < 15)
            {
                setValueByShift( 0x1, 12);
                setValueByShift( this->args[0], 0);
            }
            else
            {
                setValueByShift( 0x0, 12);
                setValueByShift( 0x00ff & (this->args[0] - this->getCmdCounter()) , 0);
            }
            if ((brnum % 15) == 0)
                setValueByShift( 0x0, 8);
            else
                setValueByShift( brnum % 15 + 1, 8);
            return 1;
        }
    }
    else if_cmd("SWI")
    {
        setValueByShift(0x11, 8);
        setValueByShift( this->args[0], 0);
        return 1;
    }
    else if_cmd("RTI")
    {
        setValueByShift(0x1110, 0);
        return 1;
    }
    else if_cmd("RETURN")
    {
        setValueByShift(0x1111, 0);
        return 1;
    }
    else if_cmd("CALL")
    {
        setValueByShift(0x2, 12);
        setValueByShift( this->args[0], 4);
        setValueByShift( this->args[1], 0);
        return 1;
    }
    // Macro commands
    else if_cmd("SET")
    {
        char d[256];
        sprintf(d, "SETLO %%r%i, %i", this->args[0], this->args[1] & 0xff);
        this->process(d, instrs, false);
        sprintf(d, "SETHI %%r%i, %i", this->args[0], this->args[1] >> 8);
        this->process(d, instrs, true);
        return 0;
    }
    else if_cmd("CMP")
    {
        char d[256];
        sprintf(d, "SETC");
        this->process(d, instrs, false);
        sprintf(d, "SUB %%r0, %%r%i, %%r%i", this->args[0], this->args[1]);
        this->process(d, instrs, true);
        return 0;
    }
    else if_cmd("NEG")
    {
        char d[256];
        sprintf(d, "SETC");
        this->process(d, instrs, false);
        sprintf(d, "SUB %%r%i, %%r0, %%r%i",this->args[0], this->args[1]);
        this->process(d, instrs, true);
        return 0;
    }
    else if_cmd("NEG")
    {
        char d[256];
        sprintf(d, "SETC");
        this->process(d, instrs, false);
        sprintf(d, "SUB %%r%i, %%r0, %%r%i",this->args[0], this->args[1]);
        this->process(d, instrs, true);
        return 0;
    }
    else if_cmd("NOT")
    {
        char d[256];
        sprintf(d, "SET %%r13, 65535");
        this->process(d, instrs, false);
        sprintf(d, "XOR %%r%i, %%r13, %%r%i",this->args[0], this->args[1]);
        this->process(d, instrs, true);
        return 0;
    }
    else if_cmd("HALT")
    {
        char d[256];
        sprintf(d, "BRR 0");
        this->process(d, instrs, false);
        return 0;
    }
    else if_cmd("NOP")
    {
        char d[256];
        sprintf(d, "BRR 1");
        this->process(d, instrs, false);
        return 0;
    }
    else if_cmd("SETC")
    {
        char d[256];
        sprintf(d, "SETF 8");
        this->process(d, instrs, false);
        return 0;
    }
    else if_cmd("CLRC")
    {
        char d[256];
        sprintf(d, "CLRF 8");
        this->process(d, instrs, false);
        return 0;
    }
    else if_cmd("SETCB")
    {
        char d[256];
        sprintf(d, "SETF 16");
        this->process(d, instrs, false);
        return 0;
    }
    else if_cmd("CLCCB")
    {
        char d[256];
        sprintf(d, "CLRF 24");
        this->process(d, instrs, false);
        return 0;
    }
    else if_cmd("FLAGS")
    {
        char d[256];
        sprintf(d, "CLRC");
        this->process(d, instrs, false);
        sprintf(d, "ADD %%r0, %%r%i, %%r0", this->args[0]);
        this->process(d, instrs, true);
        return 0;
    }
    return 0;
}

