

#ifndef ELF_INPUT_H_
#define ELF_INPUT_H_

#include <iostream>
#include <fstream>
#include <string>
//#include <bits/ios_base.h>
#include "ELF_Types.h"
#include "ELF_Structs.h"

using std::ifstream;

//const ios_base::seekdir From_begin = ios_base::beg;
//const ios_base::seekdir From_end = ios_base::end;
//const ios_base::seekdir From_cur = ios_base::cur;

//typedef ios_base::streampos sposition;
//typedef ios_base::streamoff soffset;

class ELF_Input :public ifstream
{};/*
private:
	ifstream ifile;
public:
	ELF_Input(string source);
	char get();
	char peep();
	bool read (char* buf, int size);
	virtual ~ELF_Input();
};*/


#endif /* ELF_INPUT_H_ */


