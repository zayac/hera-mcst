#include "Assembler.h"
#include "ELFWriter.h"
#include "ELF_Output.h"

int Assembler::BUFFER_SIZE = 32768;
Assembler* Assembler::instance = NULL;

Assembler::Assembler(OutputFileType fileType)
{
    objectFile = fileType;
    registerValues = vector<unsigned short int>(16, 0);
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
    unsigned lineCounter = 0;
    pc = 0;
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
                {
                    cout << str.getString() << " " << pc << endl;
                    string s1 = str.getString();
                    labels.insert(pair<string, unsigned>(s1.substr(0, s1.length() - 1), pc));
                }
            	else
            	{
                    cout << filename << ". Duplicate label in line " << lineCounter << ": \"" << str.getString() <<  "\"" << endl;
                    failbit = true;
            	}
            }
            else if (!str.isEmpty())
            {
                asmInstrs.push_back(str);
                //cout << str.getString() << " : " << str.isMacroForTwoInstructions() << endl;
                if(str.isMacroForTwoInstructions())
                {
                        pc += 2;
                }
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

    cout << "<< Instructions >>" << endl;
    pc = 0;
    for (vector<Instruction>::iterator instr = asmInstrs.begin(); instr != asmInstrs.end(); instr++)
    {
    	//cout <<  instr->getString() << endl;
        //cout << pc << endl;
    	vector< pair <unsigned short int, string> > ret = instr->encode(pc);
        pc++;
    	for (vector<pair <unsigned short int, string> >::iterator i = ret.begin(); i != ret.end(); i++)
    	{
            encodedInstructions.push_back(i->first);
            instructionStrings.push_back(i->second);
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

    if(objectFile == ELF)
        writeToFileELF(name, encodedInstructions);
    else if (objectFile == MIF)
        writeToFileMIF(name, encodedInstructions);
}


void Assembler::writeToFileELF(string filename, vector<unsigned short int>instrs)
{
    ELFWriter to;
    ELF_Output out;
    unsigned short int a[BUFFER_SIZE];
	copy( instrs.begin(), instrs.end(), a);

    out.open(filename.c_str(), ios_base::binary);
    to.Write (&out, a, sizeof (a));
    out.close();
}

void Assembler::writeToFileMIF(string filename, vector<unsigned short int>instrs)
{
    MIF_Output mif;
    mif.generateObjectFile<unsigned short int>(filename, instrs, instructionStrings);
}