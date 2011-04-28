#include "Instruction.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iostream>

using namespace std;
using namespace boost;

class Assembler;

regex Instruction::exprOperSection("^\\.[A-Za-z]+ [\x21-\x7E]*$");
regex Instruction::exprLabel("^[A-Za-z_.][A-Za-z0-9_.]*:$");
regex Instruction::exprReducedLabel("^[A-Za-z_.][A-Za-z0-9_.]*$");
regex Instruction::exprOperation("([A-Za-z0-9])+");
regex Instruction::exprArguments("((%r[0-9]{1,2})|(%fp))|([+-]?[0-9]+)");
regex Instruction::exprDecimalNumber("[+-]?[0-9]+");
regex Instruction::exprRegister("(%r[0-9]{1,2})|(%fp)");
regex Instruction::exprOperSet("^(setlo|sethi) ((%r[0-9]{1,2})|(%fp)) *, *(([+-]?[0-9]{1,3})|([A-Za-z_.][A-Za-z0-9_.]*))", regex_constants::icase);
regex Instruction::exprOperThreeAddress("^(and|or|add|sub|mult|xor) ((%r[0-9]{1,2})|(%fp)) *, *(((%r[0-9]{1,2})|(%fp))|([0-9]+)) *, *(((%r[0-9]{1,2})|(%fp))|([0-9]+))", regex_constants::icase);
regex Instruction::exprOperShift("^(lsl|lsr|lsl8|lsr8|asl|asr) ((%r[0-9]{1,2})|(%fp)) *, *(((%r[0-9]{1,2})|(%fp))|([0-9]+))", regex_constants::icase);
regex Instruction::exprOperSetClearFlags("^(setf|clrf) ((%r[0-9]{1,2})|(%fp))", regex_constants::icase);
regex Instruction::exprOperSaveRestoreFlags("^(save|rstrf) ((%r[0-9]{1,2})|(%fp))", regex_constants::icase);
regex Instruction::exprOperIncDecFlag("^(inc|dec) ((%r[0-9]{1,2})|(%fp)) *, *(((%r[0-9]{1,2})|(%fp))|([0-9]+))", regex_constants::icase);
regex Instruction::exprOperLoadStore("(^(store) ((%r[0-9]{1,2})|(%fp)) *, *((%r[0-9]{1,2})|(%fp)) *, *((%r[0-9]{1,2})|(%fp)))|(^(load) ((%r[0-9]{1,2})|(%fp)) *, *([0-9]+) *, *((%r[0-9]{1,2})|(%fp)))", regex_constants::icase);
regex Instruction::exprOperBranch("^(brr?|blr?|bger?|bler?|bgr?|buler?|bugr?|bzr?|bnzr?|bcr?|bncr?|bsr?|bnsr?|bvr?|bnvr?) ((([A-Za-z_.])([A-Za-z0-9_.])*)|((%r[0-9]{1,2})|(%fp)))", regex_constants::icase);
regex Instruction::exprOperReturn("^(return|rti)", regex_constants::icase);
regex Instruction::exprOperSwi("^(swi) [0-9]+", regex_constants::icase);
regex Instruction::exprOperCall("^(call) ((%r[0-9]{1,2})|(%fp)) *, *[0-9]+", regex_constants::icase);

// macro instructions
regex Instruction::exprOperMacroSet("^(set) ((%r[0-9]{1,2})|(%fp)) *, *(([+-]?[0-9]{1,5})|([A-Za-z_.][A-Za-z0-9_.]*))", regex_constants::icase);
regex Instruction::exprOperMacroCmpNegNot("^(cmp|neg|not) (((%r[0-9]{1,2})|(%fp))|([0-9]+)) *, *(((%r[0-9]{1,2})|(%fp))|([0-9]+))", regex_constants::icase);
regex Instruction::exprOperMacroHaltFlags("^(halt|nop|setc|clrc|setcb|clccb|)", regex_constants::icase);

Instruction::Instruction(string str) : str(str)
{
    cleanInstruction();
    for(int i = 0; i < str.length(); i++) str[i] = tolower(str[i]);
    string branches[] = { "br", "bl",  "bge",  "ble",  "bg",  "bule",  "bug",  "bz",  "bnz",  "bc",  "bnc",  "bs",  "bns",  "bv",  "bnv",
                         "brr", "blr", "bger", "bler", "bgr", "buler", "bugr", "bzr", "bnzr", "bcr", "bncr", "bsr", "bnsr", "bvr", "bnvr" };
    this->branches.insert(this->branches.begin(), branches, branches + 30);
}
Instruction::Instruction() { str = ""; }

void Instruction::removeWhiteSpaces()
{
    istringstream ss(str);
    string output, tmp;
    while(ss >> tmp) output += tmp + " ";
    str = output.substr(0, output.size()-1);
}

void Instruction::removeComment()
{
    str = str.substr(0, str.find_first_of("#", 0));
}

bool Instruction::isMacroInstruction() const
{
    return isMacroSetInstruction() || isMacroCmpNegNot() || isMacroHaltFlags() || isMacroForTwoInstructions() || isMacroForThreeInstructions();
}

void Instruction::cleanInstruction()
{
    removeComment();
    removeWhiteSpaces();
}

// Image branch name into it's index in branches vector
int Instruction::findBranch( string br)
{
    for ( unsigned int i = 0; i < this->branches.size(); i++)
    {
        if (br == this->branches[i])
            return i;
    }
    return -1;
}

optional<string> Instruction::getOperation() const
{
    match_results<string::const_iterator> what;
    if(regex_search(str, what, exprOperation))
    {
        return string( what[0].first, what[0].second );
    }
    else
        return optional<string>();
}

vector<string> Instruction::getArguments() const
{
    vector<string> ret;
    sregex_token_iterator end;
    if (isBranchInstruction())
    {
        regex brregex("%?([A-Za-z_.])([A-Za-z0-9_.])*");
        sregex_token_iterator itr(str.begin(), str.end(), brregex, 0);
        for(itr++; itr != end; ++itr) ret.push_back(string(itr->first, itr->second));
    } if (isMacroSetInstruction() || isSetInstruction()) {
        regex brregex("((%r[0-9]{1,2})|(%fp))|([A-Za-z_.][A-Za-z0-9_.]*)|([+-]?[0-9]+)");
        sregex_token_iterator itr(str.begin(), str.end(), brregex, 0);
        for(itr++; itr != end; ++itr) {
            ret.push_back(string(itr->first, itr->second));
        }
    } else {
        sregex_token_iterator itr(str.begin() + str.find_first_of(" "), str.end(), exprArguments, 0);
        for(; itr != end; ++itr)
        {
            if (string(itr->first, itr->second).compare("%fp"))
                ret.push_back(string(itr->first, itr->second));
            else
                ret.push_back(string("%r14"));
        }
    }
    return ret;
}

short Instruction::getNumFromArg(string str)
{
    match_results<string::const_iterator> what;
    if(regex_search(str, what, exprDecimalNumber))
    {
        return atoi(string( what[0].first, what[0].second ).c_str());
    }
    else
        return -1;
}

bool Instruction::isRegister() const { return regex_match(str, exprRegister); }
bool Instruction::isEmpty() const { return regex_match(str, regex("")); }
bool Instruction::isLabel() const { return regex_match(str, exprLabel); }
bool Instruction::isReducedLabel() const { return regex_match(str, exprReducedLabel); }
bool Instruction::isSetInstruction() const { return regex_match(str, exprOperSet); }
bool Instruction::isThreeAddressInstruction() const { return regex_match(str, exprOperThreeAddress); }
bool Instruction::isShiftInstruction() const { return regex_match(str, exprOperShift); }
bool Instruction::isSetClearFlagsInstruction() const { return regex_match(str, exprOperSetClearFlags); }
bool Instruction::isSaveRestoreFlagsInstruction() const { return regex_match(str, exprOperSaveRestoreFlags); }
bool Instruction::isIncDecFlagInstruction() const { return regex_match(str, exprOperIncDecFlag); }
bool Instruction::isLoadStoreInstruction() const { return regex_match(str, exprOperLoadStore); }
bool Instruction::isBranchInstruction() const { return regex_match(str, exprOperBranch); }
bool Instruction::isReturnInstruction() const { return regex_match(str, exprOperReturn); }
bool Instruction::isSwiInstruction() const { return regex_match(str, exprOperSwi); }
bool Instruction::isCallInstruction() const { return regex_match(str, exprOperCall); }
bool Instruction::isMacroSetInstruction() const { return regex_match(str, exprOperMacroSet); }
bool Instruction::isMacroCmpNegNot() const { return regex_match(str, exprOperMacroCmpNegNot); }
bool Instruction::isMacroHaltFlags() const { return regex_match(str, exprOperMacroHaltFlags); }
bool Instruction::isMacroForTwoInstructions() const { return regex_match(getOperation().get(), regex("((set)|(cmp)|(neg)|(flags))")); }
bool Instruction::isMacroForThreeInstructions() const { return regex_match(getOperation().get(), regex("(not)")); }
bool Instruction::isSection() const { return regex_match(str, exprOperSection); }

bool Instruction::validateString() const
{
    return  isEmpty() || isLabel() || isSetInstruction() || isThreeAddressInstruction() ||
            isShiftInstruction() || isSetClearFlagsInstruction() || isSaveRestoreFlagsInstruction() ||
            isIncDecFlagInstruction() || isLoadStoreInstruction() || isBranchInstruction() ||
            isReturnInstruction() || isSwiInstruction() || isCallInstruction() || isMacroSetInstruction() ||
            isMacroCmpNegNot() || isMacroHaltFlags();
}

vector< pair <unsigned short int, string> > Instruction::encode(unsigned &cmd_counter)
{
    instr_word = 0;
    static int stack = -1;
    stack++;
    for(int i = 0; i < stack; i++) cout << "\t";
    cout  << getString();
    vector<string> args = getArguments();
    vector< pair<unsigned short int, string> > result;
    if (!isMacroInstruction())
    {
        // SETLO and SETHI
        if(!getOperation().get().compare("setlo")) {
            setValueByShift( 0xE, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair <unsigned short int, string> (instr_word, getString()));
        } else if (!getOperation().get().compare("sethi")) {
            setValueByShift( 0xF, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair <unsigned short int, string> (instr_word, getString()));
        }
        // Tree-address operations
        else if (!getOperation().get().compare("and")) {
            setValueByShift( 0x1, 15);
            setValueByShift( 0x0, 12);
            setValueByShift( getNumFromArg(args[1]), 8);
            setValueByShift( getNumFromArg(args[2]), 4);
            setValueByShift( getNumFromArg(args[3]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("or")) {
            setValueByShift( 0x1, 15);
            setValueByShift( 0x1, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("add")) {
            setValueByShift( 0x1, 15);
            setValueByShift( 0x2, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("sub")) {
            setValueByShift( 0x1, 15);
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("mult")) {
            setValueByShift( 0x1, 15);
            setValueByShift( 0x4, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("xor")) {
            setValueByShift( 0x1, 15);
            setValueByShift( 0x5, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        }
        // Shifts
        else if (!getOperation().get().compare("lsl")) {
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( 0x0, 4);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("lsr")) {
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( 0x1, 4);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("lsl8")) {
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( 0x2, 4);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("lsr8")) {
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( 0x3, 4);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("asl")) {
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( 0x4, 4);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("asr")) {
            setValueByShift( 0x3, 12);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( 0x5, 4);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        }
        // Set/Clear flags
        else if (!getOperation().get().compare("setf")) {
            setValueByShift( 0x3, 12);
            setValueByShift( 0x0, 11);
            setValueByShift( (0x10 & getNumFromArg(args[0])) >> 4, 8);
            setValueByShift( 0xF & getNumFromArg(args[0]), 0);
            setValueByShift( 0x3, 5);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("clrf")) {
            setValueByShift( 0x3, 12);
            setValueByShift( 0x1, 11);
            setValueByShift( (0x10 & getNumFromArg(args[0])) >> 4, 8);
            setValueByShift( 0xF & getNumFromArg(args[0]), 0);
            setValueByShift( 0x3, 5);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        }
        // Save/restore flags
        else if (!getOperation().get().compare("savef")) {
            setValueByShift( 0x3, 12);
            setValueByShift( 0x7, 4);
            setValueByShift( 0x0, 3);
            setValueByShift( getNumFromArg(args[0]), 8);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("rstrf")) {
            setValueByShift( 0x3, 12);
            setValueByShift( 0x7, 4);
            setValueByShift( 0x1, 3);
            setValueByShift( getNumFromArg(args[0]), 8);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        }
        // Increments
        else if (!getOperation().get().compare("inc")) {
            setValueByShift( 0x3, 12);
            setValueByShift( 0x2, 6);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("dec")) {
            setValueByShift( 0x3, 12);
            setValueByShift( 0x3, 6);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( getNumFromArg(args[1]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        }
        // Memory Instructions
        else if (!getOperation().get().compare("load")) {
            setValueByShift( 0x2, 13);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( (0x10 & getNumFromArg(args[1])) >> 4, 12);
            setValueByShift(  0xF & getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
        } else if (!getOperation().get().compare("store")) {
            setValueByShift( 0x3, 13);
            setValueByShift( getNumFromArg(args[0]), 8);
            setValueByShift( (0x10 & getNumFromArg(args[1])) >> 4, 12);
            setValueByShift(  0xF & getNumFromArg(args[1]), 4);
            setValueByShift( getNumFromArg(args[2]), 0);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
            }
            // Branches, including jumps
            else if (isBranchInstruction())
            {
                int brnum = findBranch( getOperation().get());
                map<string, unsigned> labels = Assembler::getInstance()->getLabels();
                if (brnum != -1)
                {
                    if( brnum < 15)
                    {
                        setValueByShift( 0x1, 12);
                        if (Instruction(args[0]).isRegister())
                            setValueByShift(getNumFromArg(args[0]), 0);
                        else
                            setValueByShift( labels.find(args[0])->second, 0);
                    }
                    else
                    {
                        setValueByShift( 0x0, 12);
                        setValueByShift( 0x00ff & (labels.find(args[0])->second - cmd_counter) , 0);
                    }
                    if ((brnum % 15) == 0)
                        setValueByShift( 0x0, 8);
                    else
                        setValueByShift( brnum % 15 + 1, 8);
            result.push_back(pair<unsigned short int, string>(instr_word, getString()));
                }
            } else if (!getOperation().get().compare("swi")) {
                setValueByShift(0x11, 8);
                setValueByShift( getNumFromArg(args[0]), 0);
                result.push_back(pair<unsigned short int, string>(instr_word, getString()));
            } else if (!getOperation().get().compare("rti")) {
                setValueByShift(0x1110, 0);
            } else if (!getOperation().get().compare("return")) {
                setValueByShift(0x1111, 0);
                result.push_back(pair<unsigned short int, string>(instr_word, getString()));
            } else if (!getOperation().get().compare("call")) {
                setValueByShift(0x2, 12);
                setValueByShift( getNumFromArg(args[0]), 4);
                setValueByShift( getNumFromArg(args[1]), 0);
                result.push_back(pair<unsigned short int, string>(instr_word, getString()));
            }
            if (result.size() == 1) cout << "\t\t0x" << hex << result[0].first << endl;
    }
    else {
        cout << endl;
        // Macro commands
        if (!getOperation().get().compare("set")) {
            char d[256];
            if (Instruction(args[1]).isReducedLabel())
            {
                Assembler* assm = Assembler::getInstance();
                char tmp[5];
                map<string, unsigned> labels = assm->getLabels();
                if (labels.find(args[1]) != labels.end())
                {
                    sprintf(tmp,"%d", labels.find(args[1])->second);
                    args[1] = tmp;
                }
                else
                {
                    cout << "Error. A label " + args[1] + " is not defined." << endl;
                    exit(-1);
                }
            }
            sprintf(d, "setlo %%r%i, %i", getNumFromArg(args[0]), getNumFromArg(args[1]) & 0xff);
            vector< pair <unsigned short int, string > > v = Instruction(d).encode(cmd_counter);
            cmd_counter++;
            for (vector< pair <unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
            sprintf(d, "sethi %%r%i, %i", getNumFromArg(args[0]), getNumFromArg(args[1]) >> 8);
            v = Instruction(d).encode(cmd_counter);
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("cmp")) {
            char d[256];
            sprintf(d, "setc");
            vector< pair<unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            cmd_counter++;
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
            sprintf(d, "sub %%r0, %%r%i, %%r%i", getNumFromArg(args[0]), getNumFromArg(args[1]));
            v = Instruction(d).encode(cmd_counter);
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("neg")) {
            char d[256];
            sprintf(d, "setc");
            vector< pair <unsigned short int, string > > v = Instruction(d).encode(cmd_counter);
            cmd_counter++;
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
            sprintf(d, "sub %%r%i, %%r0, %%r%i", getNumFromArg(args[0]), getNumFromArg(args[1]));
            v = Instruction(d).encode(cmd_counter);
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("not")) {
            char d[256];
            sprintf(d, "set %%r13, 65535");
            vector< pair <unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            cmd_counter++;
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
            sprintf(d, "xor %%r%i, %%r13, %%r%i", getNumFromArg(args[0]), getNumFromArg(args[1]));
            v = Instruction(d).encode(cmd_counter);
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("halt")) {
            char d[256];
            sprintf(d, "brr 0");
            vector< pair <unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            cmd_counter++;
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("nop")) {
            char d[256];
            sprintf(d, "brr 1");
            vector< pair<unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            for (vector< pair<unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("setc")) {
            char d[256];
            sprintf(d, "setf 8");
            vector< pair<unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            for (vector< pair < unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("clrc")) {
            char d[256];
            sprintf(d, "clr 8");
            vector< pair< unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            for (vector< pair< unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        } else if (!getOperation().get().compare("setcb")) {
            char d[256];
            sprintf(d, "setf 16");
            vector< pair< unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            for (vector<pair < unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
            } else if (!getOperation().get().compare("clccb")) {
                char d[256];
                sprintf(d, "clrf 24");
                vector< pair< unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
                for (vector<pair <unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                    result.push_back(*i);
        } else if (!getOperation().get().compare("flags")) {
            char d[256];
            sprintf(d, "clrc");
            vector< pair<unsigned short int, string> > v = Instruction(d).encode(cmd_counter);
            cmd_counter++;
            for (vector<pair <unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
            sprintf(d, "add %%r0, %%r%i, %%r0", getNumFromArg(args[0]));
            v = Instruction(d).encode(cmd_counter);
            for (vector<pair <unsigned short int, string> >::iterator i = v.begin(); i != v.end(); i++)
                result.push_back(*i);
        }
        cout << endl;
    }
    stack--;
    return result;
}
