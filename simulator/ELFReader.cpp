#include "ELFReader.h"

#define RET_ERROR(err) {_last_error = err; return false;}
#define TRY_OR_RET(exp, err) if (!(exp)) RET_ERROR(err)

ELFReader::ELFReader (unsigned short int *memory)
:_memory (memory)
{
    _last_error = EEno_error;
    //ctor
}

ELFReader::~ELFReader()
{
    //dtor
}

bool ELFReader::read (ELF_Input* fr)
{
    _from = fr;
    TRY_OR_RET(_from->read ((char*)&_fheader, sizeof (_fheader)), EEeheader_not_readed);
    if (_fheader.e_ident[0] != 0x7f ||
        _fheader.e_ident[1] != 'E' || //identification
        _fheader.e_ident[2] != 'L' ||
        _fheader.e_ident[3] != 'F' )   RET_ERROR(EEunrecognized_file_format);
    if (_fheader.e_ehsize != sizeof (_fheader)) RET_ERROR(EEeheader_not_readed);

    if (!read_sections()) return false;
    if (_fheader.e_phoff != 0) //if file containes program segments,
        if (!read_prog_headers()) return false; //read it!

    TRY_OR_RET (read_program_dump(), EENo_program_bits);

    _last_error = EEno_error;
    return true;
}

bool ELFReader::read_sections()
{
    if (_fheader.e_shoff == 0) RET_ERROR(EEhas_not_sections);
    if (sizeof (Elf32_Shdr) != _fheader.e_shentsize)
        RET_ERROR(EEshentsize_not_correspond);

    Elf32_Shdr sheader;
    _from->seekg (_fheader.e_shoff);

    for (int i = 0; i < _fheader.e_shnum; ++i) //load each section header,
    {                                         //and remember it
        TRY_OR_RET(_from->read ((char*)&sheader, _fheader.e_shentsize), EECould_not_read_sheader);
        _sheaders.push_back (sheader);
    }
    return true;
}

bool ELFReader::read_prog_headers()
{
    if (sizeof (Elf32_Phdr) != _fheader.e_phentsize)
        RET_ERROR(EEphentsize_not_correspond);

    Elf32_Phdr pheader;
    _from->seekg (_fheader.e_phoff);

    for (int i = 0; i < _fheader.e_phnum; ++i)
    {
        TRY_OR_RET(_from->read ((char*)&pheader, _fheader.e_phentsize), EECoudl_not_read_pheader);
        _pheaders.push_back (pheader);
    }

    return true;
}

bool ELFReader::read_program_dump()
{
    for (list <Elf32_Shdr>::iterator i = _sheaders.begin(); i != _sheaders.end(); ++i)
        if (i->sh_type = SHT_PROGBITS)
            {
                _from->seekg (i->sh_offset);//go to section
                _from->read ((char*)_memory, i->sh_size);
                return true;
            }
    return false;
}
