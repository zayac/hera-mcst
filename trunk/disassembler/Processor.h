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

public:
    // Get and set data by dedicated address
	word Get (mem_operator pos) const;
	word Set (mem_operator pos, word val);

	// return pointer to the memory array
    word* data(){return _data;}
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

class Executor :public Execution
{
    bool _pc_increase_needed;
	word _reg[REG_NUM];         // Register file
	word  PC;                   // Program counter

    // Flag register
	union
	{
		struct
		{
			unsigned char sign:1;
			unsigned char zero:1;
			unsigned char overflow:1;
			unsigned char carry:1;
			unsigned char carry_block:1;
		};
		struct
		{
			int flags:5;
		};
	};
	word& SP;               // Stack pointer
	word& FP;               // Frame pointer
	word& TMP;

	Memory* _mem;          // Pointer to a memory

	inline void Correct_zero_sign (reg_ind d);
	bool Get_condition_for_branch (half_byte c);

public:
    Executor (Memory* _mem);
    /* Print some debugging information.
     * Only register file at the moment
     */ 
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

    // Set program counter value
	virtual void Set_PC (word nval);
    // Continue performing instructions from the stopped place
	virtual bool Continue();
	virtual word Get_cur_cmd();
	virtual void End_circle();

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
    Translator (Execution* ex);
    void Set_execution (Execution* ex) {_exe = ex;}
    // Perform a single instruction
	void Perform (word cmd);
    
    // Run all instructions in a sequence starting with the first instruction
	void Run (word begin);
};

