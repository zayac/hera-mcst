#include "Processor.h"
#include <iostream>

#define PC_INCREASE_NEEDED true
#define PC_INCREASE_NOT_NEEDED false

inline word Get_add_code (word that) {return (~that + 1);}
inline dword Get_add_code (dword that) {return (~that + 1);}

//---------------------------------------------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------------------------------------------
void Translator::Print_hex_cmd(word command)
{
	word part_cmd;

	cout << "0x";
	for(int k=1; k<=(sizeof(word)*2); command<<=4, k++){
		part_cmd = ((command & 0xf000)>>12);

		switch (part_cmd)
		{
			case 0xa:cout<<'a';break;
			case 0xb:cout<<'b';break;
			case 0xc:cout<<'c';break;
			case 0xd:cout<<'d';break;
			case 0xe:cout<<'e';break;
			case 0xf:cout<<'f';break;
			default: cout<<part_cmd;
		}
	}
	cout << "\t";
}
//---------------------------------------------------------------------------------------------------------------
void Translator::Run (word begin)
{
    _exe->Set_PC (begin);
	while (_exe->Continue() && _exe->Get_cur_cmd())
	{
	    Perform (_exe->Get_cur_cmd());
        _exe->dump();
		_exe->End_circle();
	}

}
//---------------------------------------------------------------------------------------------------------------
Translator::Translator (Execution* ex) :_exe (ex)
{}
//---------------------------------------------------------------------------------------------------------------
void Translator::Perform (word cmd)
{
	half_byte opcode = (cmd >> 12)&0xf;

	Print_hex_cmd(cmd);

	if					(Perform_arithmetic (cmd)) return;
	else if (opcode == 0x3){//0011
			if			(Perform_shift_inc_flags (cmd)) return;}
	else if ((opcode&0xC) == 0x4)//01xx
	{
                         Process_load_store (cmd);
        return;
	}


	switch (opcode)
	{
		/*1110*/case 0xE:
		/*1111*/case 0xF:
			Process_set (cmd);
			break;
		/*0001*/case 0x1:
		/*0000*/case 0x0:
			if		((cmd&0xfff0) == 0x1100)//0001 0001 0000 i
				_exe->SWI (cmd&0x000f);
			else if (cmd == 0x1110)//0001 0001 0001 0000
				_exe->RTI();
			else if (cmd == 0x1111)//0001 0001 0001 0001
				_exe->RETURN();
			else
				Process_branch (cmd);
			break;
		/*0010*/case 0x2:
			_exe->CALL ( (cmd&0x0ff0)>>4, cmd&0x000f);
			break;
	};
}
//---------------------------------------------------------------------------------------------------------------
inline bool Translator::Perform_arithmetic (word cmd)
{
	reg_ind d = (cmd>>8)&0xf;
	reg_ind a = (cmd>>4)&0xf;
	reg_ind b =  cmd	&0xf;

	half_byte opcode = (cmd >> 12)&0xf;

	switch (opcode)
	{
		/*1000*/case 0x8: _exe->AND (d, a, b);	return true;
		/*1001*/case 0x9: _exe->OR (d, a, b);	return true;
		/*1010*/case 0xA: _exe->ADD (d, a, b);	return true;
		/*1011*/case 0xB: _exe->SUB (d, a, b);	return true;
		/*1100*/case 0xC: _exe->MULT (d, a, b);	return true;
		/*1101*/case 0xD: _exe->XOR (d, a, b);	return true;
	};
	return false;
}
//---------------------------------------------------------------------------------------------------------------
inline bool Translator::Perform_shift_inc_flags (word cmd)
{
	reg_ind d = (cmd>>8)&0xf;
	reg_ind b =  cmd	&0xf;
	half_byte opcode = (cmd >> 4)&0xf;

	switch (opcode)
	{
		/*0000*/case 0x0: _exe->LSL (d, b);	return true;
		/*0001*/case 0x1: _exe->LSR (d, b);	return true;
		/*0010*/case 0x2: _exe->LSL8 (d, b);return true;
		/*0011*/case 0x3: _exe->LSR8 (d, b);return true;
		/*0100*/case 0x4: _exe->ASL (d, b);	return true;
		/*0101*/case 0x5: _exe->ASR (d, b);	return true;

		/*0110*/case 0x6: Process_set_clr_flags (cmd); return true;
		/*0111*/case 0x7: Process_save_restore  (cmd); return true;
	};
	if ((opcode&0x8) != 0)
	{
		Process_inc_dec (cmd);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------
inline void Translator::Process_set_clr_flags (word cmd)
{
	five_bits b = (cmd&0x000f) | ((cmd&0x0100) >> 4); //0000 000a 0000 bcde -> abcde
	if ((cmd&0x0800) == 0) //0000 1000 0000 0000
		_exe->SETF (b);
	else
		_exe->CLRF (b);
}
//---------------------------------------------------------------------------------------------------------------
inline void Translator::Process_save_restore (word cmd)
{
	reg_ind d = (cmd&0x0f00) >> 8;
	if ((cmd&0x0008) == 0) //0000 0000 0000 1000
		_exe->SAVEF (d);
	else
		_exe->RSTRF (d);
}
//---------------------------------------------------------------------------------------------------------------
inline void Translator::Process_inc_dec (word cmd)
{
	byte v = cmd&0x003f;//0000 0000 0011 1111
	reg_ind d = (cmd&0x0f00) >> 8;

	if ((cmd&0x0040) == 0) //0000 0000 0100 0000
		_exe->INC (d, v);
	else
		_exe->DEC (d, v);
}
//---------------------------------------------------------------------------------------------------------------
inline void Translator::Process_set (word cmd)
{
	byte v = cmd&0x00ff;//0000 0000 1111 1111
	reg_ind d = (cmd&0x0f00) >> 8;

	if ((cmd&0x1000) == 0) //0001 0000 0000 0000
		_exe->SETLO (d, v);
	else
		_exe->SETHI (d, v);
}
//---------------------------------------------------------------------------------------------------------------
inline void Translator::Process_load_store (word cmd)
{
	reg_ind b = cmd&0x000f;//0000 0000 0011 1111
	reg_ind d = (cmd&0x0f00) >> 8;
	five_bits o = ((cmd&0x00f0) >> 4) | ((cmd&0x1000) >> 8); //000a 0000 bcde 0000 -> abcde

	if ((cmd&0x2000) == 0) //0010 0000 0000 0000
		_exe->LOAD (d, o, b);
	else
		_exe->STORE (d, o, b);
}
//---------------------------------------------------------------------------------------------------------------
inline void Translator::Process_branch (word cmd)
{
	half_byte c = (cmd&0x0f00) >> 8;
	if ((cmd&0x1000) == 0)
	{
		byte r = cmd&0x00ff;//0000 0000 0011 1111
		_exe->BRANCHR (c, r);
	}
	else
	{
		reg_ind b = cmd&0x000f;//0000 0000 0011 1111
		_exe->BRANCH (c, b);
	}
}

//---------------------------------------------------------------------------------------------------------------
word Memory::Get (mem_operator pos) const
{
	return _data[pos];
}
//---------------------------------------------------------------------------------------------------------------
word Memory::Set (mem_operator pos, word val)
{
	return _data[pos] = val;
}
//---------------------------------------------------------------------------------------------------------------
