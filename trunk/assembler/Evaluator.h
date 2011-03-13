#include <string>
#include <boost/regex.hpp>
#include <boost/optional/optional.hpp>

using namespace std;
using namespace boost;

class Evaluator
{
	static regex exprLabel;						// a regular expression that evaluates a label
	static regex exprOperation;					// a regular expression that evaluates an operation
	static regex exprDecimalNumber;					// a regular expression that evaluates a number
	static regex exprRegister;						// a regular expression that evaluates a register

	// the following regex relates to instruction match
	static regex exprOperSet;					// SETLO and SETHI
	static regex exprOperThreeAddress;			// Three-address operations
	static regex exprOperShift;					// shift operations
	static regex exprOperSetClearFlags;			// set/clear flags operations
	static regex exprOperSaveRestoreFlags;		// save/restore flags operations
	static regex exprOperIncDecFlag;			// incDec flags operations
	static regex exprOperLoadStore;				// load/store operations
	static regex exprOperBranch;				// branch operations
	static regex exprOperReturn;				// return operations
	static regex exprOperSwi;					// software interrupt operations
	static regex exprOperCall;					// call instruction
public:
	//bool checkString(string str);				// checks if an instruction string is correct
	string removeWhiteSpaces(string str);		// removes all unnecessary special symbols from an instruction string
	string removeComment(string str);			// removes comment from an instruction string
	string cleanInstruction(string str);		// removes comments and whitespaces

	/*
	 * optional<string> is used for setting a "null" string
	 * get an operation name
	 */
	optional<string> getOperation(string str);
	bool isLabel(string str);							// check if an instruction is a label for branch
	bool isSetInstruction(string str);					// check if an instruction is a set instruction
	bool isthreeAddressInstruction(string str);			// check if an instruction is a three-address instruction
	bool isShiftInstruction(string str);				// check if an instruction is a shift instruction
	bool isSetClearFlagsInstruction(string str);		// check if an instruction is a ser/clear flags instruction
	bool isSaveRestoreFlagsInstruction(string str);		// check if an instruction is a saver/restore flags instruction
	bool isIncDecFlagInstruction(string str);			// check if an instruction is a inc/dec flag instruction
	bool isLoadStoreInstruction(string str);			// check if an instruction is a load/store instruction
	bool isBranchInstruction(string str);				// check if an instruction is a branch instruction
	bool isReturnInstruction(string str);				// check if an instruction is a return instruction
	bool isSwiInstruction(string str);					// check if an instruction is a software interrupt instruction
	bool isCallInstruction(string str);					// check if an instruction is a call instruction
};
