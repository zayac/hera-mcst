#include "CELFHeader.h"

ELFHeader::ELFHeader() {
	e_ident[EI_MAG0] = ELFMAG0;
	e_ident[EI_MAG1] = ELFMAG1;
	e_ident[EI_MAG2] = ELFMAG2;
	e_ident[EI_MAG3] = ELFMAG3;
	e_ident[EI_CLASS] = ELFCLASSNONE;
	e_ident[EI_DATA] = ELFDATANONE;
	e_ident[EI_VERSION] = EV_NONE;
	e_ident[EI_PAD] = 0;
	e_ident[EI_NIDENT] = sizeof(Elf32_Ehdr);
}

void ELFHeader::setDefault() {
	e_ident[EI_CLASS] = ELFCLASS32;
	e_ident[EI_DATA] = ELFDATA2LSB;
	e_ident[EI_VERSION] = EV_CURRENT;
}

ELFHeader::~ELFHeader() {

}
