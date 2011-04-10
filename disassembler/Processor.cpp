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
//sleep(1);
}
//---------------------------------------------------------------------------------------------------------------
void Translator::Run (word begin)
{
    _exe->Set_PC (begin);
	while (_exe->Continue())
	{cout<<"|"<<_exe->Continue()<<"|"<<_exe->Get_cur_cmd()<<"|"<< _exe->_PC <<"|"<<endl;
	    Perform (_exe->Get_cur_cmd());
        _exe->dump();
		_exe->End_circle();
sleep(1);
	}

}
//---------------------------------------------------------------------------------------------------------------
Translator::Translator (Disasm* ex) :_exe (ex)
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

	if ((cmd&0x0020) == 0) //0000 0000 0010 0000
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
/*Executor::Executor (Memory* mem)
:flags(0x10), SP(_reg[15]), FP (_reg[14]), TMP(_reg[13]), _mem (mem)
{
    for (int i = 0; i < REG_NUM; i++)
        _reg[i] = 0;
}

void Executor::dump()
{
    for(int i = 0; i < REG_NUM; i++)
        cout << _reg[i] << " ";
    cout << endl;
}
//---------------------------------------------------------------------------------------------------------------
inline void Executor::Correct_zero_sign (reg_ind d)
{
	zero = (_reg[d]&0xffff) == 0;
	sign = (_reg[d]&0x8000) != 0;//0b1000 0000 0000 0000
}
//---------------------------------------------------------------------------------------------------------------
bool Executor::Get_condition_for_branch (half_byte c)
{
	switch (c)
	{
		case 0x0: return true;
		case 0x2: return sign ^ overflow;
		case 0x3: return !(sign^overflow);
		case 0x4: return (sign^overflow)||zero;
		case 0x6: return (!carry)||zero;
		case 0x7: return !((!carry)||zero);
		case 0x8: return zero;
		case 0x9: return !zero;
		case 0xA: return carry;
		case 0xB: return !carry;
		case 0xC: return sign;
		case 0xD: return !sign;
		case 0xE: return overflow;
		case 0xF: return !overflow;
		default: return false;
	};
}
//---------------------------------------------------------------------------------------------------------------
void Executor::SETLO (reg_ind d, byte v)
{
	_reg[d] = v | _reg[d]&0xff00;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::SETHI (reg_ind d, byte v)
{
	_reg[d] = word(v)<<8 | _reg[d]&0x00ff;//not tested !!!;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::AND (reg_ind d, reg_ind a, reg_ind b)
{
	_reg[d] = _reg[a]&_reg[b];
	Correct_zero_sign (d);
}
//---------------------------------------------------------------------------------------------------------------
void Executor::OR (reg_ind d, reg_ind a, reg_ind b)
{
	_reg[d] = _reg[a]|_reg[b];
	Correct_zero_sign (d);
}
//---------------------------------------------------------------------------------------------------------------
#ifndef abs
word abs (int what) {return what > 0 ? what : -what;}
#endif
void Executor::ADD (reg_ind d, reg_ind a, reg_ind b)
{
/*	const word sign_mask = 0x8000;//1000 0000 0000 0000
	bool signa = _reg[a]&sign_mask >> 15;
	bool signb = _reg[b]&sign_mask >> 15;
	word aval = _reg[a]&0x7fff;
	word bval = _reg[b]&0x7fff;//0111 1111 1111 1111

	if (sizeof (int) < sizeof (word)) return; // it's possible to store any register in int

	int rez = int(aval)*(signa? -1:1) + int(bval)*(signb? -1:1) + (carry && !carry_block);

	_reg[d] = (word)(abs(rez)&0x5fff) | (rez < 0 ? 0x8000 : 0);//ставим знаковый бит
	Correct_zero_sign (d);
	overflow = carry = rez>>15;*/

/*	dword rez = dword (_reg[a]) + dword (_reg[b]);
	_reg[d] = rez&0xffff;
	Correct_zero_sign(d);
    overflow = ((0x1&(_reg[b]>>15)) == (0x1&(_reg[a]>>15)) && (0x1&(rez>>15)) != (0x1&(_reg[b]>>15)));
    carry = 0x1&(rez>>16);
	//!!!overflow may be incorrect
}
//---------------------------------------------------------------------------------------------------------------
void Executor::SUB (reg_ind d, reg_ind a, reg_ind b)
{
/*	const word sign_mask = 0x8000;//1000 0000 0000 0000
	bool signa = _reg[a]&sign_mask >> 15;
	bool signb = _reg[b]&sign_mask >> 15;
	word aval = _reg[a]&0x7fff;
	word bval = _reg[b]&0x7fff;//0111 1111 1111 1111

	if (sizeof (int) < sizeof (word)) return; // it's possible to store any register in int

	int rez = int(aval)*(signa? -1:1) - int(bval)*(signb? -1:1) - (carry && !carry_block);

	_reg[d] = (word)(abs(rez)&0x5fff) | word(rez < 0 ? 0x8000 : 0); // set sign bit
	Correct_zero_sign (d);
	overflow = carry = rez>>15;*/

/*	word antib = Get_add_code (_reg[b]);//conversion fo additional code

	dword rez = dword (_reg[a]) + dword (antib);
	_reg[d] = rez&0xffff;
	Correct_zero_sign(d);

    overflow = ((0x1&(antib>>15)) == (0x1&(_reg[a]>>15)) && (0x1&(rez>>15)) != (0x1&(antib>>15)));
    carry = 0x1&(rez>>16);

	//!!!overflow may be incorrect
}
//---------------------------------------------------------------------------------------------------------------
void Executor::MULT (reg_ind d, reg_ind a, reg_ind b)
{
/*	const word sign_mask = 0x8000;//1000 0000 0000 0000
	bool signa = _reg[a]&sign_mask >> 15;
	bool signb = _reg[b]&sign_mask >> 15;
	word aval = _reg[a]&0x7fff;
	word bval = _reg[b]&0x7fff;//0111 1111 1111 1111

	dword rez = (dword)aval*(dword)bval;
	_reg[d] = word(rez);
	TMP = word(rez>>16)|word(signa != signb ? 0x8000 : 0);
	zero = rez == 0;
	sign = signa != signb;
	overflow = (TMP&0x7fff) != 0; // doesn't count sign bit
	//carry not defined
	//??? Where will be sign stored, if the register is set to reg[d] */

/*	const word sign_mask = 0x8000;//1000 0000 0000 0000
	bool signa = (_reg[a]&sign_mask) >> 15;
	bool signb = (_reg[b]&sign_mask) >> 15;
	word aval = _reg[a]&0xffff;
	word bval = _reg[b]&0xffff;
	if (signa)
        aval = Get_add_code (aval);
    if (signb)
        bval = Get_add_code (bval);

	dword rez = (dword)aval*(dword)bval;

	sign = signa != signb;
	zero = rez == 0;
	overflow = (rez>>16) != 0;

	if (sign) rez = Get_add_code (rez);

	_reg[d] = word(rez&0xffff);
	TMP = word((rez>>16)&0xffff);
}
//---------------------------------------------------------------------------------------------------------------
void Executor::XOR (reg_ind d, reg_ind a, reg_ind b)
{
	_reg[d] = _reg[a]^_reg[b];
	Correct_zero_sign (d);
}
//---------------------------------------------------------------------------------------------------------------
void Executor::LSL (reg_ind d, reg_ind v)
{
	bool next_carry = (_reg[v] & 0x8000) != 0;
	_reg[d] = _reg[v]<<1 | (carry && !carry_block);
	carry = next_carry == true;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::LSR (reg_ind d, reg_ind v)
{
	bool next_carry = (_reg[v] & 0x0001) != 0;
	_reg[d] = _reg[v]>>1 | ((carry && !carry_block)<<15);
	carry = next_carry == true;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::LSL8 (reg_ind d, reg_ind v)
{
	_reg[d] = (_reg[v]<<8) & 0xffff;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::LSR8 (reg_ind d, reg_ind v)
{
	_reg[d] = (_reg[v]>>8) & 0xffff;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::ASL (reg_ind d, reg_ind v)
{
	_reg[d] = (carry && !carry_block) | ((_reg[v]<<1) & 0x7fff) | (_reg[v] & 0x8000); // save sign
}
//---------------------------------------------------------------------------------------------------------------
void Executor::ASR (reg_ind d, reg_ind v)
{
	_reg[d] = ((_reg[v] & 0x7fff) >>1) | (_reg[v] & 0x8000); // save sign
}
//---------------------------------------------------------------------------------------------------------------
void Executor::SETF (five_bits b)
{
	flags = flags|b; //!!!unverified. Could be bad conversion
}
//---------------------------------------------------------------------------------------------------------------
void Executor::CLRF (five_bits b)
{
	flags = flags&b; //!!!unverified. Could be bad conversion
}
//---------------------------------------------------------------------------------------------------------------
void Executor::SAVEF (reg_ind d)
{
	_reg[d] = flags; //!!!unverified. Could be bad conversion
}
//---------------------------------------------------------------------------------------------------------------
void Executor::RSTRF (reg_ind d)
{
	flags = _reg[d]; //!!!unverified. Could be bad conversion
}
//---------------------------------------------------------------------------------------------------------------
void Executor::INC (reg_ind d, byte v)
{
	carry = overflow = (dword(_reg[d]) + dword (v) + 1) >>16 != 0;
	_reg[d] += v + 1;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::DEC (reg_ind d, byte v)
{
	carry = overflow = _reg[d] < (v+1);
	_reg[d] -= v + 1;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::LOAD (reg_ind d, five_bits o, reg_ind b)
{
	_reg[d] = _mem->Get (_reg[b] + o);
}
//---------------------------------------------------------------------------------------------------------------
void Executor::STORE (reg_ind d, five_bits o, reg_ind b)
{
	_mem->Set (_reg[b] + o, _reg[d]);
}
//---------------------------------------------------------------------------------------------------------------
void Executor::BRANCH (half_byte c, reg_ind b)
{
	if (Get_condition_for_branch (c))
	{
	    PC = _reg[b];
        _pc_increase_needed = false;
	}
}
//---------------------------------------------------------------------------------------------------------------
void Executor::BRANCHR (half_byte c, byte r)
{
	if (Get_condition_for_branch (c))
	{
	    PC += (char) r;
        _pc_increase_needed = false;
	}
}
//---------------------------------------------------------------------------------------------------------------
#include <iostream>
void Executor::SWI (half_byte i)
{
    if (i == 0) PC = 0;//Terminate
    if (i == 1) { std::cout <<"r13 = " <<_reg[13] <<std::endl; PC++;}//!!! wrong, but useless
	//!!!
	_pc_increase_needed = false;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::RTI()
{
	//!!!
	_pc_increase_needed = false;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::RETURN()
{
	SP = FP;
	PC = _mem->Get (SP);
	FP = TMP;
	_pc_increase_needed = false;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::CALL (byte s, reg_ind b)
{
	_mem->Set (SP, PC + 1);
	PC = _reg[b];
	TMP = FP;
	FP = SP;
	SP = SP + s;
	_pc_increase_needed = false;
}
//---------------------------------------------------------------------------------------------------------------
void Executor::Set_PC (word nval)
{
    PC = nval;
	_pc_increase_needed = true;
}
//---------------------------------------------------------------------------------------------------------------
bool Executor::Continue()
{
    return PC != 0;
}
//---------------------------------------------------------------------------------------------------------------
word Executor::Get_cur_cmd()
{
    return _mem->Get (PC);
}
//---------------------------------------------------------------------------------------------------------------
void Executor::End_circle()
{
    if (_pc_increase_needed) ++PC;
    _pc_increase_needed = true;
    //dump();
}*/
Disasm::Disasm (ostream& out, Memory* mem):_out (out), _mem(mem)
{
    //ctor
}
//---------------------------------------------------------------------------------------------------------------
Disasm::~Disasm()
{
    //dtor
}
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
void Disasm::SETLO (reg_ind d, byte v)
{
    _out <<"SETLO r" <<(int)d <<", " <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::SETHI (reg_ind d, byte v)
{
    _out <<"SETHI r" <<(int)d <<", " <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::AND (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"AND r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::OR (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"OR r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::ADD (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"ADD r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::SUB (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"SUB r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::MULT (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"MULT r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::XOR (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"XOR r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::LSL (reg_ind d, reg_ind v)
{
    _out <<"LSL r" <<(int)d <<", r" <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::LSR (reg_ind d, reg_ind v)
{
    _out <<"LSR r" <<(int)d <<", r" <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::LSL8 (reg_ind d, reg_ind v)
{
    _out <<"LSL8 r" <<(int)d <<", r" <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::LSR8 (reg_ind d, reg_ind v)
{
    _out <<"LSR8 r" <<(int)d <<", r" <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::ASL (reg_ind d, reg_ind v)
{
    _out <<"ASL r" <<(int)d <<", r" <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::ASR (reg_ind d, reg_ind v)
{
    _out <<"ASR r" <<(int)d <<", r" <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::SETF (five_bits b)
{
    _out <<"SETF " <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::CLRF (five_bits b)
{
    _out <<"CLRF " <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::SAVEF (reg_ind d)
{
    _out <<"SAVEF r" <<(int)d <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::RSTRF (reg_ind d)
{
    _out <<"RSTRF r" <<(int)d <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::INC (reg_ind d, byte v)
{
    _out <<"INC r" <<(int)d <<", " <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::DEC (reg_ind d, byte v)
{
    _out <<"DEC r" <<(int)d <<", " <<(int)v <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::LOAD (reg_ind d, five_bits o, reg_ind b)
{
    _out <<"LOAD r" <<(int)d <<", " <<(int)o <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::STORE (reg_ind d, five_bits o, reg_ind b)
{
    _out <<"STORE r" <<(int)d <<", " <<(int)o <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
//--
char* Get_code (half_byte c)
{
    switch (c)
    {
        case 0b0000: return "BR";
        case 0b0010: return "BL";
        case 0b0011: return "BGE";
        case 0b0100: return "BLE";
        case 0b0101: return "BG";
        case 0b0110: return "BULE";
        case 0b0111: return "BUG";

        case 0b1000: return "BZ";
        case 0b1001: return "BNZ";
        case 0b1010: return "BC";
        case 0b1011: return "BNC";
        case 0b1100: return "BS";;
        case 0b1101: return "BNS";
        case 0b1110: return "BV";
        case 0b1111: return "BNV";
    };
}
//---------------------------
void Disasm::BRANCH (half_byte c, reg_ind b)
{
    _out <<Get_code (c) <<" r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::BRANCHR (half_byte c, byte r)
{
    if (c == 0 && r == 0) return;//!!!may be not???
    _out <<Get_code (c) <<"R " <<(int)(char)r <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::SWI (half_byte i)
{
    _out <<"SWI " <<(int)i <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::RTI()
{
    _out <<"RTI" <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::RETURN()
{
    _out <<"RETURN" <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::CALL (byte s, reg_ind b)
{
    _out <<"CALL "<<(int)s <<", r" <<(int)b <<endl;
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::Set_PC (word nval)
{
    _PC = nval;
}
//---------------------------------------------------------------------------------------------------------------
bool Disasm::Continue()
{
    return ( (_PC < 0xffff) && _out.good()) && (_PC != 0);
}
//---------------------------------------------------------------------------------------------------------------
word Disasm::Get_cur_cmd()
{
    return _mem->Get (_PC);
}
//---------------------------------------------------------------------------------------------------------------
void Disasm::End_circle()
{
    _PC++;
    if (_PC > 0xffff) _PC = 0xffff;
}

void Disasm::dump()
{
}
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
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
