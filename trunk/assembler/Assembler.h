#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "ELFReader.h"
#include "Instruction.h"
#include "MIF_Output.h"

using namespace std;

class Instruction;


class Assembler
{
public:
    enum OutputFileType { ELF, MIF };
private:
    string filename;
    unsigned pc;
    vector<unsigned short int> encodedInstructions;
    ELF_Input ifile;
    vector<Instruction> asmInstrs;
    map<string, unsigned> labels;
    void open(string ifile);					    // stands just for output file
    static int BUFFER_SIZE;
    static Assembler* instance;
    OutputFileType objectFile;
    Assembler(OutputFileType fileType = ELF);
    void writeToFileELF(string filename, vector<unsigned short int>instrs);
    void writeToFileMIF(string filename, vector<unsigned short int>instrs);
public:
    inline OutputFileType getObjectFileType() const { return objectFile; }
    inline void setObjectFileType(OutputFileType type) { objectFile = type; }
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
