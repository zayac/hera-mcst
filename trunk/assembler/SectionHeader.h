#ifndef SECTIONHEADER_H_
#define SECTIONHEADER_H_

#include "ELF_Types.h"
#include "ELF_Structs.h"

class SectionHeader :protected Elf32_Shdr
{
public:
    SectionHeader(Elf32_Word name, Elf32_Word type, Elf32_Off offset, Elf32_Word size);
    ~SectionHeader();

	Elf32_Shdr* data() {return (Elf32_Shdr*)this;}
	const Elf32_Shdr* data() const {return (const Elf32_Shdr*)this;}
};

#endif /* SECTIONHEADER_H_ */

