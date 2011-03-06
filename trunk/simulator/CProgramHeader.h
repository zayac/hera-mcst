#ifndef PROGRAMHEADER_H_
#define PROGRAMHEADER_H_

#include "ELF_Types.h"
#include "ELF_Structs.h"

class ProgramHeader :protected Elf32_Phdr
{
public:
	ProgramHeader();
	void setDefault();
	virtual ~ProgramHeader();


	Elf32_Phdr* data() {return (Elf32_Phdr*)this;}
	const Elf32_Phdr* data() const {return (const Elf32_Phdr*)this;}
};

#endif /* PROGRAMHEADER_H_ */
