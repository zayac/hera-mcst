#include <iostream>
#include <string>
#include "Instruction.h"
#include "ELFWriter.h"
#include "ELFReader.h"
#include <cstring>
// nre vommrn
using namespace std;

/*
 * This assembler is used to generate an object file that represents instructions as numbers
 * from asm text file.
 * There should be the only argument which is path to a asm file, e.g. test.s   
 */
int main(int argc, char** argv)
{
    /* ELF_Input should be a module for a convinient assembler file reading,
     * yet right now it's just the same as istream
     */
    ELF_Input ifile;

    if (argc == 1)
    {
        cout << "There should be at least one file name argument" << endl;
        return 0;
    }
    int i = 0;
    while(argv[++i])
    {
        if(!ifstream(argv[i]))
        {
            cout << "File " << argv[i] << " doesn't exist!" << endl;
            break;
        }
        else
        {
            ifile.open(argv[i]);

            if(ifile.fail())
            {
                cout << "Error with opening file " << argv[i] << endl;
                break;
            }

            /* The following array represents encoded instuctions.
             * We assume that each instruction's size is 16 bit,
             * and the number of instructions should be less than 16 bit (2^16)
             */
            unsigned short int instrs[32768];
            memset(instrs, 0, sizeof(instrs));

            // This is the object that encodes instructions
            Instruction parser;                       
            string str;
            
            // Each instruction is read and processed separatly
            while(!ifile.eof())
            {
                getline(ifile, str);
                parser.process(str.c_str(), instrs, true);
            }
            ifile.close();

            // The remaining code stands just for output file
            string name (argv[i]);
            size_t found = name.rfind(".");
            if (found!=string::npos)
                name.replace (found + 1, name.length() - found - 1, "o");
            else
            {
                cout << "Error in file " << argv[i] << endl;
            }

            ELFWriter to;
            ELF_Output out;
            out.open(name.c_str(), ios_base::binary);
            to.Write (&out, instrs, sizeof (instrs));
            out.close();

            cout << argv[i] << " is translated successfully" << endl;
        }
    }
    return 0;
}

