#pragma once
#ifndef ELFWRITER_H_INCLUDED
#define ELFWRITER_H_INCLUDED

#include "ELF_Output.h"

class ELFWriter
{
    public:
        ELFWriter();
        virtual ~ELFWriter();

        bool Write (ELF_Output *out, unsigned short int *mem, int memsize);
    protected:
    private:
};

#endif // ELFWRITER_H_INCLUDED
