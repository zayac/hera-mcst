#include <iostream>
#include <string>
#include "Assembler.h"
#include "ELFWriter.h"
#include "ELFReader.h"
#include <cstring>
#include "Instruction.h"
using namespace std;

/*
 * This assembler is used to generate an object file that represents instructions as numbers
 * from asm text file.
 * There should be one or more arguments which is path to a asm file, e.g. test.s
 */
int main(int argc, char** argv)
{
    if (argc == 1)
    {
        cout << "There should be at least the file name argument" << endl;
        return 0;
    }
    int i = 0;
    Assembler* assemb = Assembler::getInstance();
    bool objectFileTypeIsSet = false;
    for(i = 2; i < argc; i++)
    {
        if (string(argv[i]) == "--mif" )
        {
            assemb->setObjectFileType(Assembler::MIF);
            if(!objectFileTypeIsSet)
                objectFileTypeIsSet = true;
            else
                cout << "Duplicate object file type detected: " << argv[i] << endl;
        }
        else if(string(argv[i]) == "--elf")
        {
            assemb->setObjectFileType(Assembler::ELF);
            if(!objectFileTypeIsSet)
                objectFileTypeIsSet = true;
            else
                cout << "Duplicate object file type detected: " << argv[i] << endl;
        }
    }

    ifstream ifile(argv[1]);
    if (ifile) {
        assemb->run(argv[1]);
        assemb->writeToFile();
    }
    else
    {
        cout << "File " << argv[i] << "doesn't exist" << endl;
    }

}

