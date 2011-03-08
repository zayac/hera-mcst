#include <iostream>
#include <string>
#include "ELF_Output.h"
#include "ELF_Input.h"
#include "CELFHeader.h"
#include "Instruction.h"
#include "CProgramHeader.h"
#include "SectionHeader.h"
#include "ELFWriter.h"
#include "ELFReader.h"
//#include "Processor.h"
#include "Disasm.h"
#include <fstream>
#include <cstring>

using namespace std;

/* An application is used for running object files generated by assembler
 * The only argument is an object file
 */
int main(int argc, char** argv)
{
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
            Memory mem;
            // constructor that loads application to the memory
            ELFReader rdr(mem.data());
            ELF_Input from;

            from.open (argv[i], ios_base::binary);
            if (rdr.read (&from)) cout <<"read successefull!"<<endl;
            else cout <<rdr.Get_last_error();
            from.close();


            Executor ex(&mem);
            Disasm dis (cout, &mem);
            Translator cpu(&ex);
            //    cpu.Set_execution (&dis);
            //cpu.Run (1);

            cout <<"disasm:\n";
            cpu.Set_execution (&dis);
            cpu.Run (1);
            //cpu._exe.dump
        }
    }
    return 0;
}

