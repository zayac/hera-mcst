#include "CProgramHeader.h"

ProgramHeader::ProgramHeader()
{
	p_type 	= PT_NULL;
	p_offset = 0;
	p_vaddr	= 0;
	p_paddr	= 0;
	p_filesz = 0;
	p_memsz	= 0;
	p_flags	= 0;
	p_align	= 0;
}

ProgramHeader::~ProgramHeader() {
	// TODO Auto-generated destructor stub
}
