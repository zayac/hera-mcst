#include "Evaluator.h"
#include <boost/regex.hpp>
#include <sstream>
#include <iostream>

using namespace std;
using namespace boost;

regex Evaluator::exprLabel("^([A-Za-z_.])([A-Za-z0-9_.])*:$");
regex Evaluator::exprOperation("([A-Za-z])+");
regex Evaluator::exprDecimalNumber("[0-9]+");
regex Evaluator::exprRegister("%r[0-9]{1,2}");
regex Evaluator::exprOperSet("^(setlo|sethi) (%r[0-9]{1,2}) *, *((%r[0-9]{1,2})|([0-9]+))", regex_constants::icase);
regex Evaluator::exprOperThreeAddress("^(and|or|add|sub|mult|xor) (%r[0-9]{1,2}) *, *((%r[0-9]{1,2})|([0-9]+)) *, *((%r[0-9]{1,2})|([0-9]+))", regex_constants::icase);
regex Evaluator::exprOperShift("^(lsl|lsr|lsl8|lsr8|asl|asr) (%r[0-9]{1,2}) *, *((%r[0-9]{1,2})|([0-9]+))", regex_constants::icase);
regex Evaluator::exprOperSetClearFlags("^(setf|clrf) (%r[0-9]{1,2})", regex_constants::icase);
regex Evaluator::exprOperSaveRestoreFlags("^(save|rstrf) (%r[0-9]{1,2})", regex_constants::icase);
regex Evaluator::exprOperIncDecFlag("^(inc|dec) (%r[0-9]{1,2}) *, *((%r[0-9]{1,2})|([0-9]+))", regex_constants::icase);
regex Evaluator::exprOperLoadStore("^(load|store) (%r[0-9]{1,2}) *, *((%r[0-9]{1,2})|([0-9]+)) *, *((%r[0-9]{1,2})|([0-9]+))", regex_constants::icase);
regex Evaluator::exprOperBranch("^(brr?|blr?|bger?|bler?|bgr?|buler?|bugr?|bzr?|bnzr?|bcr?|bncr?|bsr?|bnsr?|bvr?|bnvr?) ([A-Za-z_.])([A-Za-z0-9_.])*", regex_constants::icase);
regex Evaluator::exprOperReturn("^(return|rti)", regex_constants::icase);
regex Evaluator::exprOperSwi("^(swi) [0-9]+", regex_constants::icase);
regex Evaluator::exprOperCall("^(call) (%r[0-9]{1,2}) *, *[0-9]+", regex_constants::icase);

string Evaluator::removeWhiteSpaces(string str)
{
	istringstream ss(str);
	string output, tmp;
	while(ss >> tmp) output += tmp + " ";
	return output.substr(0, output.size()-1);;
}

string Evaluator::removeComment(string str)
{
	return str.substr(0, str.find_first_of("#", 0));
}

string Evaluator::cleanInstruction(string str)
{
	return removeWhiteSpaces(removeComment(str));
}

optional<string> Evaluator::getOperation(string str)
{
	match_results<string::const_iterator> what;
	if(regex_search(str, what, exprOperation))
	{
		return string( what[0].first, what[0].second );
	}
	else
		return optional<string>();
}

bool Evaluator::isLabel(string str) { return regex_match(str, exprLabel); }
bool Evaluator::isSetInstruction(string str) { return regex_match(str, exprOperSet); }
bool Evaluator::isthreeAddressInstruction(string str) { return regex_match(str, exprOperThreeAddress); }
bool Evaluator::isShiftInstruction(string str) { return regex_match(str, exprOperShift); }
bool Evaluator::isSetClearFlagsInstruction(string str) { return regex_match(str, exprOperSetClearFlags); }
bool Evaluator::isSaveRestoreFlagsInstruction(string str) { return regex_match(str, exprOperSaveRestoreFlags); }
bool Evaluator::isIncDecFlagInstruction(string str) { return regex_match(str, exprOperIncDecFlag); }
bool Evaluator::isLoadStoreInstruction(string str) { return regex_match(str, exprOperBranch); }
bool Evaluator::isBranchInstruction(string str) { return regex_match(str, exprOperBranch); }
bool Evaluator::isReturnInstruction(string str) { return regex_match(str, exprOperReturn); }
bool Evaluator::isSwiInstruction(string str) { return regex_match(str, exprOperSwi); }
bool Evaluator::isCallInstruction(string str) { return regex_match(str, exprOperCall); }
