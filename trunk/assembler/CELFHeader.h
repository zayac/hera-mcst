#ifndef ELF_HEADER_H_
#define ELF_HEADER_H_

#include "ELF_Types.h"
#include "ELF_Structs.h"

class ELFHeader :public Elf32_Ehdr
{
public:
	ELFHeader();
	void setDefault();
	virtual ~ELFHeader();

	Elf32_Ehdr* data() {return (Elf32_Ehdr*)this;}
	const Elf32_Ehdr* data() const {return (const Elf32_Ehdr*)this;}
};

#endif /* ELF_HEADER_H_ */
