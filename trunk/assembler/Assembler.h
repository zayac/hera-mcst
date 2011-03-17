#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <vector>
#include <map>
#include "Instruction.h"
#include <string>
#include "ELFReader.h"

using namespace std;

class Assembler
{
	string filename;
	unsigned pc;
	vector<unsigned short int> encodedInstructions;
	ELF_Input ifile;
	vector<Instruction> asmInstrs;
	map<string, unsigned> labels;
	void open(string ifile);
	void writeToFile();						    // stands just for output file
	static int BUFFER_SIZE;
public:
	Assembler();
	void run(string ifile);

};

#endif /* ASSEMBLER_H_ */
