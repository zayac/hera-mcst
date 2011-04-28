#include "ELF_Input.h"

class MIFReader{
    protected:
        unsigned short int *_memory;
        unsigned int *_Size_Program;
    public:
        MIFReader(unsigned short int *memory, unsigned int *Size_Program);
        bool read(ELF_Input* file);
};
