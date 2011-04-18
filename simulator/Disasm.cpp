#include "Disasm.h"

using std::endl;


Disasm::Disasm (ostream& out, Memory* mem):_out (out), _mem(mem)
{
    //ctor
}

//------------------------------------------------------------------------------

Disasm::~Disasm()
{
    //dtor
}

//------------------------------------------------------------------------------

void Disasm::SETLO (reg_ind d, byte v)
{
    _out <<"SETLO r" <<(int)d <<", " <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::SETHI (reg_ind d, byte v)
{
    _out <<"SETHI r" <<(int)d <<", " <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::AND (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"AND r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::OR (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"OR r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::ADD (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"ADD r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::SUB (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"SUB r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::MULT (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"MULT r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::XOR (reg_ind d, reg_ind a, reg_ind b)
{
    _out <<"XOR r" <<(int)d <<", r" <<(int)a <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::LSL (reg_ind d, reg_ind v)
{
    _out <<"LSL r" <<(int)d <<", r" <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::LSR (reg_ind d, reg_ind v)
{
    _out <<"LSR r" <<(int)d <<", r" <<(int)v <<endl;
}
//------------------------------------------------------------------------------
void Disasm::LSL8 (reg_ind d, reg_ind v)
{
    _out <<"LSL8 r" <<(int)d <<", r" <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::LSR8 (reg_ind d, reg_ind v)
{
    _out <<"LSR8 r" <<(int)d <<", r" <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::ASL (reg_ind d, reg_ind v)
{
    _out <<"ASL r" <<(int)d <<", r" <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::ASR (reg_ind d, reg_ind v)
{
    _out <<"ASR r" <<(int)d <<", r" <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::SETF (five_bits b)
{
    _out <<"SETF " <<(int)b <<endl;
}

//------------------------------------------------------------------------------
void Disasm::CLRF (five_bits b)
{
    _out <<"CLRF " <<(int)b <<endl;
}
//------------------------------------------------------------------------------

void Disasm::SAVEF (reg_ind d)
{
    _out <<"SAVEF r" <<(int)d <<endl;
}

//------------------------------------------------------------------------------

void Disasm::RSTRF (reg_ind d)
{
    _out <<"RSTRF r" <<(int)d <<endl;
}

//------------------------------------------------------------------------------

void Disasm::INC (reg_ind d, byte v)
{
    _out <<"INC r" <<(int)d <<", " <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::DEC (reg_ind d, byte v)
{
    _out <<"DEC r" <<(int)d <<", " <<(int)v <<endl;
}

//------------------------------------------------------------------------------

void Disasm::LOAD (reg_ind d, five_bits o, reg_ind b)
{
    _out <<"LOAD r" <<(int)d <<", " <<(int)o <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::STORE (reg_ind d, five_bits o, reg_ind b)
{
    _out <<"STORE r" <<(int)d <<", " <<(int)o <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

char* Get_code (half_byte c)
{
    switch (c)
    {
        case 0x00: return "BR";
        case 0x02: return "BL";
        case 0x03: return "BGE";
        case 0x04: return "BLE";
        case 0x05: return "BG";
        case 0x06: return "BULE";
        case 0x07: return "BUG";

        case 0x08: return "BZ";
        case 0x09: return "BNZ";
        case 0x0a: return "BC";
        case 0x0b: return "BNC";
        case 0x0c: return "BS";;
        case 0x0d: return "BNS";
        case 0x0e: return "BV";
        case 0x0f: return "BNV";
    };
} 

//------------------------------------------------------------------------------ 
/* 
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
*/
//------------------------------------------------------------------------------

void Disasm::BRANCH (half_byte c, reg_ind b)
{
    _out <<Get_code (c) <<" r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::BRANCHR (half_byte c, byte r)
{
    if (c == 0 && r == 0) return;//!!!may be not???
    _out <<Get_code (c) <<"R " <<(int)(char)r <<endl;
}

//------------------------------------------------------------------------------

void Disasm::SWI (half_byte i)
{
    _out <<"SWI " <<(int)i <<endl;
}

//------------------------------------------------------------------------------

void Disasm::RTI()
{
    _out <<"RTI" <<endl;
}

//------------------------------------------------------------------------------

void Disasm::RETURN()
{
    _out <<"RETURN" <<endl;
}

//------------------------------------------------------------------------------

void Disasm::CALL (byte s, reg_ind b)
{
    _out <<"CALL "<<(int)s <<", r" <<(int)b <<endl;
}

//------------------------------------------------------------------------------

void Disasm::Set_PC (word nval)
{
    _PC = nval;
}

//------------------------------------------------------------------------------

bool Disasm::Continue()
{
    return _PC < 0xffff && _out.good();
}

//------------------------------------------------------------------------------

word Disasm::Get_cur_cmd()
{
    return _mem->Get (_PC);
}

//------------------------------------------------------------------------------

void Disasm::End_circle()
{
    _PC++;
    if (_PC > 0xffff) _PC = 0xffff;
}

//------------------------------------------------------------------------------

void Disasm::dump()
{
	
}

//------------------------------------------------------------------------------
