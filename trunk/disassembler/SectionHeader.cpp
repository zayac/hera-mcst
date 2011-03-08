#include "SectionHeader.h"

SectionHeader::SectionHeader(Elf32_Word name, Elf32_Word type, Elf32_Off offset, Elf32_Word size) {
      sh_name = name;
      sh_type = type;
     sh_flags = 0;
      sh_addr = 0;
    sh_offset = offset;
      sh_size = size;
      sh_link = SHN_UNDEF;
      sh_info = 0;
 sh_addralign = 0;
   sh_entsize = 0;
}

SectionHeader::~SectionHeader() {

}
