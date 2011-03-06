#ifndef ELF_OUTPUT_H_
#define ELF_OUTPUT_H_

#include <iostream>
#include <fstream>
#include <string>
#include "ELF_Types.h"
#include "ELF_Structs.h"

using namespace std;

class ELF_Output:public ofstream
{};
/* {
private:
	ofstream ofile;
public:
	ELF_Output(string source);
	void write(char * buffer, int size);
	virtual ~ELF_Output();
};
*/


#endif /* ELF_OUTPUT_H_ */
