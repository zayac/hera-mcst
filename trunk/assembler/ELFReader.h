#pragma once
#ifndef ELFREADER_H
#define ELFREADER_H

#include <list>
#include "ELF_Structs.h"
#include "ELF_Types.h"
#include "ELF_Input.h"

using std::list;

enum ELF_error
{
    EEno_error = 0,
    EEeheader_not_readed,
    EEunrecognized_file_format,
    EEhas_not_sections,
    EEshentsize_not_correspond,
    EECould_not_read_sheader,
    EEphentsize_not_correspond,
    EECoudl_not_read_pheader,
    EENo_program_bits
};

class ELFReader
{
private:
    bool read_sections();
    bool read_prog_headers();

    bool read_program_dump();

    ELF_Input* _from;
    ELF_error _last_error;

protected:
    Elf32_Ehdr _fheader;
    list <Elf32_Shdr> _sheaders;
    list <Elf32_Phdr> _pheaders;

    unsigned short int *_memory;


    public:
        ELFReader (unsigned short int *memory);
        virtual ~ELFReader();

        bool read (ELF_Input* from);
        inline ELF_error Get_last_error() const {return _last_error;}
};

#endif // ELFREADER_H
