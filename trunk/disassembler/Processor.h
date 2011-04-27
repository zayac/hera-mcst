#pragma once
const int MEM_SIZE = 65536;
const int REG_NUM = 16;
typedef unsigned int dword;
typedef unsigned short int word;
typedef unsigned char byte;
typedef unsigned char half_byte;
typedef unsigned char five_bits;
typedef unsigned int mem_operator;
typedef half_byte reg_ind;

class Memory
{
	word _data[MEM_SIZE];
	unsigned int Size_Program;
public:
    // Get and set data by dedicated address
	word Get (mem_operator pos) const;
	word Set (mem_operator pos, word val);

	// return pointer to the memory array
    word* data(){return _data;}
   unsigned int * Get_size(){return &Size_Program;}
};

/* 
 * This is a pure abstract class
 * This is done because there are Simulation and Disassembler versions of this class.
 */
class Execution
{
    public:

    virtual void dump() = 0;

	virtual void SETLO (reg_ind d, byte v) = 0;
	virtual void SETHI (reg_ind d, byte v) = 0;

	virtual void AND (reg_ind d, reg_ind a, reg_ind b) = 0;
	virtual void OR (reg_ind d, reg_ind a, reg_ind b) = 0;
	virtual void ADD (reg_ind d, reg_ind a, reg_ind b) = 0;
	virtual void SUB (reg_ind d, reg_ind a, reg_ind b) = 0;
	virtual void MULT (reg_ind d, reg_ind a, reg_ind b) = 0;
	virtual void XOR (reg_ind d, reg_ind a, reg_ind b) = 0;

	virtual void LSL (reg_ind d, reg_ind v) = 0;
	virtual void LSR (reg_ind d, reg_ind v) = 0;
	virtual void LSL8 (reg_ind d, reg_ind v) = 0;
	virtual void LSR8 (reg_ind d, reg_ind v) = 0;
	virtual void ASL (reg_ind d, reg_ind v) = 0;
	virtual void ASR (reg_ind d, reg_ind v) = 0;

	virtual void SETF (five_bits b) = 0;
	virtual void CLRF (five_bits b) = 0;

	virtual void SAVEF (reg_ind d) = 0;
	virtual void RSTRF (reg_ind d) = 0;

	virtual void INC (reg_ind d, byte v) = 0;
	virtual void DEC (reg_ind d, byte v) = 0;

	virtual void LOAD (reg_ind d, five_bits o, reg_ind b) = 0;
	virtual void STORE (reg_ind d, five_bits o, reg_ind b) = 0;

	virtual void BRANCH (half_byte c, reg_ind b) = 0;
	virtual void BRANCHR (half_byte c, byte r) = 0;
	virtual void SWI (half_byte i) = 0;
	virtual void RTI() = 0;
	virtual void RETURN() = 0;

	virtual void CALL (byte s, reg_ind b) = 0;

	virtual void Set_PC (word nval) = 0;
	virtual bool Continue() = 0;
	virtual word Get_cur_cmd() = 0;
	virtual void End_circle() = 0;
};


class Translator
{
    Execution* _exe;

	inline bool Perform_arithmetic (word cmd);          // Arithmetic instructions
	inline bool Perform_shift_inc_flags (word cmd);
	inline void Process_set_clr_flags (word cmd);       // Set/clear flags instructions
	inline void Process_save_restore (word cmd);        // Save/restore instructions
	inline void Process_inc_dec (word cmd);             // Increment/Decrement instructions
	inline void Process_set (word cmd);                 // Set instructions
	inline void Process_load_store (word cmd);          // Load/Store instructions
	inline void Process_branch (word cmd);              // Branch instructions

public:
    void Print_hex_cmd(word command);
    Translator (Execution* ex);
    void Set_execution (Execution* ex) {_exe = ex;}
    // Perform a single instruction
	void Perform (word cmd);
    
    // Run all instructions in a sequence starting with the first instruction
	void Run (word begin);
};

