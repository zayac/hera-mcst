#include "ELFWriter.h"
#include "CELFHeader.h"
#include "CProgramHeader.h"
#include "SectionHeader.h"

ELFWriter::ELFWriter()
{
    //ctor
}

ELFWriter::~ELFWriter()
{
    //dtor
}

bool ELFWriter::Write (ELF_Output *out, unsigned short int *mem, int memsize)
{
	ELFHeader header;
	ProgramHeader p_header;
	SectionHeader section(1, SHT_PROGBITS, sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr), memsize);

    header.e_ehsize = sizeof (Elf32_Ehdr);

    header.e_phoff = sizeof (Elf32_Ehdr);
    header.e_phentsize = sizeof (Elf32_Phdr);
    header.e_phnum = 1;

    header.e_shoff = sizeof (Elf32_Ehdr) + sizeof (Elf32_Phdr) + memsize;
    header.e_shentsize = sizeof (Elf32_Shdr);
    header.e_shnum = 1;


    out->write((char*) header.data(), sizeof (Elf32_Ehdr));
    out->write((char*) p_header.data(), sizeof (Elf32_Phdr));
    out->write((char*) mem, memsize);
    //cout << "Write: " << sizeof (Elf32_Ehdr) + sizeof (Elf32_Phdr) + memsize << endl;
    out->write((char*) section.data(), sizeof(Elf32_Shdr));
    return true;
}
