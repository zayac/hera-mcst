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
        cout << "There should be at least one file name argument" << endl;
        return 0;
    }
    int i = 0;
    Assembler assemb;
    while(argv[++i])
    {
    	assemb.run(argv[i]);
    }
}

