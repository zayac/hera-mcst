#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <vector>
#include <map>
#include "Instruction.h"
#include <string>
#include "ELFReader.h"

using namespace std;

class Instruction;

class Assembler
{
    string filename;
    unsigned pc;
    vector<unsigned short int> encodedInstructions;
    ELF_Input ifile;
    vector<Instruction> asmInstrs;
    map<string, unsigned> labels;
    void open(string ifile);					    // stands just for output file
    static int BUFFER_SIZE;
    static Assembler* instance;
    Assembler();
public:
    static Assembler* getInstance()
    {
        if (instance == NULL)
            return instance = new Assembler();
        return instance;
    }

    map<string, unsigned> getLabels() const { return labels; }
    void run(string ifile);
    void writeToFile();

};

#endif /* ASSEMBLER_H_ */
