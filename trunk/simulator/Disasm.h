#pragma once
#ifndef DISASM_H
#define DISASM_H

#include "Processor.h"
#include <iostream>

using std::ostream;

class Disasm : public Execution
{
    ostream &_out;
    Memory* _mem;
    int _PC;

    public:
    Disasm (ostream& out, Memory* mem);
    virtual ~Disasm();

    virtual void dump();
	virtual void SETLO (reg_ind d, byte v);
	virtual void SETHI (reg_ind d, byte v);

	virtual void AND (reg_ind d, reg_ind a, reg_ind b);
	virtual void OR (reg_ind d, reg_ind a, reg_ind b);
	virtual void ADD (reg_ind d, reg_ind a, reg_ind b);
	virtual void SUB (reg_ind d, reg_ind a, reg_ind b);
	virtual void MULT (reg_ind d, reg_ind a, reg_ind b);
	virtual void XOR (reg_ind d, reg_ind a, reg_ind b);

	virtual void LSL (reg_ind d, reg_ind v);
	virtual void LSR (reg_ind d, reg_ind v);
	virtual void LSL8 (reg_ind d, reg_ind v);
	virtual void LSR8 (reg_ind d, reg_ind v);
	virtual void ASL (reg_ind d, reg_ind v);
	virtual void ASR (reg_ind d, reg_ind v);

	virtual void SETF (five_bits b);
	virtual void CLRF (five_bits b);

	virtual void SAVEF (reg_ind d);
	virtual void RSTRF (reg_ind d);

	virtual void INC (reg_ind d, byte v);
	virtual void DEC (reg_ind d, byte v);

	virtual void LOAD (reg_ind d, five_bits o, reg_ind b);
	virtual void STORE (reg_ind d, five_bits o, reg_ind b);

	virtual void BRANCH (half_byte c, reg_ind b);
	virtual void BRANCHR (half_byte c, byte r);
	virtual void SWI (half_byte i);
	virtual void RTI();
	virtual void RETURN();

	virtual void CALL (byte s, reg_ind b);

	virtual void Set_PC (word nval);
	virtual bool Continue();
	virtual word Get_cur_cmd();
	virtual void End_circle();
};

#endif // DISASM_H
