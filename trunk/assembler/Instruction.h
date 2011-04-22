#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <string>
#include <boost/regex.hpp>
#include <boost/optional/optional.hpp>
#include "Assembler.h"
#include <iomanip>

using namespace std;
using namespace boost;

class Instruction
{
    string str;
    unsigned short int instr_word;
    vector<string> branches;

    static regex exprLabel;							// a regular expression that evaluates a label
    static regex exprReducedLabel;
    static regex exprOperation;						// a regular expression that evaluates an operation
    static regex exprArguments;						// a regular expression that evaluates arguments
    static regex exprDecimalNumber;					// a regular expression that evaluates a number
    static regex exprRegister;						// a regular expression that evaluates a register

    // the following regex relates to instruction match
    static regex exprOperSet;						// SETLO and SETHI
    static regex exprOperThreeAddress;				// Three-address operations
    static regex exprOperShift;						// shift operations
    static regex exprOperSetClearFlags;				// set/clear flags operations
    static regex exprOperSaveRestoreFlags;			// save/restore flags operations
    static regex exprOperIncDecFlag;				// incDec flags operations
    static regex exprOperLoadStore;					// load/store operations
    static regex exprOperBranch;					// branch operations
    static regex exprOperReturn;					// return operations
    static regex exprOperSwi;						// software interrupt operations
    static regex exprOperCall;						// call instruction
    // these are macro instructions
    static regex exprOperMacroSet;					// set instruction
    static regex exprOperMacroCmpNegNot;			// cmp, neg or not instruction
    static regex exprOperMacroHaltFlags;			// halt instruction and flags instructions
    static regex exprOperSection;

    void removeWhiteSpaces();						// removes all unnecessary special symbols from an instruction string
    void removeComment();							// removes comment from an instruction string
    void cleanInstruction();						// removes comments and whitespaces

    void setValueByShift( int value, int shift) { this->instr_word = instr_word + (value << shift); }

    int findBranch( string br);						// Image branch name into it's index in branches vector
public:
    Instruction(string str);
    Instruction();
    bool validateString() const ;					// checks if an instruction string is correct
    string inline getString() const { return str; }
    unsigned short int getInstructionWord() const { return instr_word; }
    vector< pair <unsigned short int, string> > encode(unsigned &cmd_counter);

    /*
     * optional<string> is used for setting a "null" string
     * get an operation name
     */
    optional<string> getOperation() const;
    vector<string> getArguments() const;
    static short getNumFromArg(string str);
    bool isSection() const;
    bool isEmpty() const;
    bool isLabel() const;								// check if an instruction is a label for branch
    bool isReducedLabel() const;
    bool isSetInstruction() const;						// check if an instruction is a set instruction
    bool isMacroInstruction() const;
    bool isThreeAddressInstruction() const;				// check if an instruction is a three-address instruction
    bool isShiftInstruction() const;					// check if an instruction is a shift instruction
    bool isSetClearFlagsInstruction() const;			// check if an instruction is a ser/clear flags instruction
    bool isSaveRestoreFlagsInstruction() const;			// check if an instruction is a saver/restore flags instruction
    bool isIncDecFlagInstruction() const;				// check if an instruction is a inc/dec flag instruction
    bool isLoadStoreInstruction() const;				// check if an instruction is a load/store instruction
    bool isBranchInstruction() const;					// check if an instruction is a branch instruction
    bool isReturnInstruction() const;					// check if an instruction is a return instruction
    bool isRegister() const;
    bool isSwiInstruction() const;						// check if an instruction is a software interrupt instruction
    bool isCallInstruction() const;						// check if an instruction is a call instruction
    bool isMacroSetInstruction() const;					// check if an instruction is a macro set instruction
    bool isMacroCmpNegNot() const;						// check if an instruction is a macro cmp/neg/not instruction
    bool isMacroHaltFlags() const;						// check if an instruction is a macro halt or flags instruction
    bool isMacroForTwoInstructions() const;				// check in an instruction is a macro, that can be devided into two instructions
    bool isMacroForThreeInstructions() const;			// check in an instruction is a macro, that can be devided into three instructions
};


#endif /* INSTRUCTION_H_ */
