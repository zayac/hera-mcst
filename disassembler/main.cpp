#include <iostream>
#include <string>
#include "ELF_Input.h"
#include "CELFHeader.h"
#include "CProgramHeader.h"
#include "SectionHeader.h"
#include "ELFReader.h"
#include "MIFReader.h"
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

		ELF_Input from;
		from.open (argv[i], ios_base::binary);

		char check[4];
		from.read(check, 4);
		if(check[0] == 0x7f &&
		check[1] == 'E' && //preliminary identification
		check[2] == 'L' &&
		check[3] == 'F' ){
			from.seekg(0);
			ELFReader rdr(mem.data());
			if (rdr.read (&from)) cout <<"read successefull!"<<endl;
			else cout <<rdr.Get_last_error();
		} else {
			from.seekg(0);
			MIFReader rdr(mem.data(), mem.Get_size());
			if(!rdr.read (&from)) cout <<"read successefull!"<<endl;
			else cout <<"Failed read the file"<<endl;
		}
            from.close();

            Disasm dis (cout, &mem);
            Translator cpu(&dis);

            cout <<"disasm:\n";
            cpu.Run (0);
            //cpu._exe.dump
        }
    }
    return 0;
}

