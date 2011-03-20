#include "Assembler.h"
#include "ELFWriter.h"
#include "ELF_Output.h"

int Assembler::BUFFER_SIZE = 32768;

Assembler::Assembler()
{
}
void Assembler::open(string str)
{
	filename = str;
    ifile.open(str.c_str());
    if(ifile.fail())
    {
        cout << "Error while opening file " << str << endl;
    }
}

void Assembler::run(string s)
{
	open(s);
	Instruction str;
	string tmps;
	unsigned lineCounter = 1;
	pc = 1;
    bool failbit = false;
    while(!ifile.eof())
    {
        getline(ifile, tmps);
        str = tmps;
        if (str.validateString())
        {
            if(str.isLabel())
            {
            	if (labels.find(str.getString()) == labels.end())
            		labels.insert(pair<string, unsigned>(str.getString(), pc));
            	else
            	{
                	cout << filename << ". Duplicate label in line " << lineCounter << ": \"" << str.getString() <<  "\"" << endl;
                	failbit = true;
            	}
            }
            else if (!str.isEmpty())
            {
        		asmInstrs.push_back(str);
        		if(str.isMacroForTwoInstructions())
        			pc += 2;
        		else if (str.isMacroForThreeInstructions())
        			pc +=3;
        		else
        			pc++;
            }
        }
        else
        {
        	if (str.isSection())
        	{
            	cout << filename << ". Warning in line " << lineCounter << ": \"" << str.getString() <<  "\". Section detected." << endl;
        	}
        	else
        	{
        		cout << filename << ". Error in line " << lineCounter << ": \"" << str.getString() <<  "\"" << endl;
        		failbit = true;
        	}
        }
        lineCounter++;
    }
    if (failbit) return;

    ifile.close();

    pc = 1;
    for (vector<Instruction>::iterator instr = asmInstrs.begin(); instr != asmInstrs.end(); instr++)
    {
    	//cout <<  instr->getString() << endl;
    	vector<unsigned short int> ret = instr->encode(pc++);
    	for (vector<unsigned short int>::iterator i = ret.begin(); i != ret.end(); i++)
    	{
    		encodedInstructions.push_back(*i);
    		//cout << "\t" << *i << endl;
    	}
    }
}

void Assembler::writeToFile()
{
    string name = filename;
    size_t found = name.rfind(".");
    if (found!=string::npos)
        name.replace (found + 1, name.length() - found - 1, "o");
    else
    {
        cout << "Error in file " << filename << endl;
    }

    ELFWriter to;
    ELF_Output out;
    unsigned short int a[BUFFER_SIZE];
	copy( encodedInstructions.begin(), encodedInstructions.end(), a);

    out.open(name.c_str(), ios_base::binary);
    to.Write (&out, a, sizeof (a));
    out.close();
}
