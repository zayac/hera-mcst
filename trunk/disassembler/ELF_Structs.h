#ifndef ELF_STRUCTS_H_INCLUDED
#define ELF_STRUCTS_H_INCLUDED

#include "ELF_Types.h"
/* ------------------------*/
/*       ELF Header        */
/* ------------------------*/

#define EI_NIDENT     16                    // Size of e_ident[]

typedef struct {
        unsigned char   e_ident[EI_NIDENT]; // The initial bytes mark the file as an object file
        Elf32_Half      e_type;             // The object file type
        Elf32_Half      e_machine;          // The architecture for an individual file
        Elf32_Word      e_version;          // The object file version
        Elf32_Addr      e_entry;            // The virtual address to which system first transfers control
        Elf32_Off       e_phoff;            // The program header table's file offset in bytes
        Elf32_Off       e_shoff;            // The section header table's file offset in bytes
        Elf32_Word      e_flags;            // Processor-specific flags associated with the file
        Elf32_Half      e_ehsize;           // The ELF header's size in bytes
        Elf32_Half      e_phentsize;        // The size in bytes of one entry in the file's program header table
        Elf32_Half      e_phnum;            // The number of entries in the program header table
        Elf32_Half      e_shentsize;        // A section header's size in bytes
        Elf32_Half      e_shnum;            // The number of entries in the section header table
        Elf32_Half      e_shstrndx;         // The section header table index of the entry associated with the section name string table
} Elf32_Ehdr;

/* ----------------------------*/
/*       Section Header        */
/* ----------------------------*/

typedef struct {
        Elf32_Word sh_name;         // The name of the section
        Elf32_Word sh_type;         // The section's content and semantics
        Elf32_Word sh_flags;        // 1-bit flags thar describe miscellaneous attributes
        Elf32_Addr sh_addr;         // Address at which the section's first byte should reside
        Elf32_Off  sh_offset;       // The byte offset from the beginning of the file to the first byte of the section
        Elf32_Word sh_size;         // The section's size in bytes
        Elf32_Word sh_link;         // A section header table index link
        Elf32_Word sh_info;         // Extra infomation, whose interpretation depends on the section type
        Elf32_Word sh_addralign;    // Address alignment constraints
        Elf32_Word sh_entsize;      // The size in bytes of each entry
} Elf32_Shdr;

/* --------------------------------*/
/*       Symbol Table Entry        */
/* --------------------------------*/

typedef struct {
        Elf32_Word    st_name;
        Elf32_Addr    st_value;     // index into the object file’s symbol string table, which holds the character representations of the symbol names
        Elf32_Word    st_size;      // a data object's size is the number of bytes contained in the object
        unsigned char st_info;      // the symbol's type and binding attributes
        unsigned char st_other;     // holds 0 and has no defined meaning
        Elf32_Half    st_shndx;     // holds the relevant section header table index
} Elf32_Sym;

/* --------------------------------*/
/*       Relocation Entries        */
/* --------------------------------*/

typedef struct {
        Elf32_Addr  r_offset;   // gives the location at which to apply the relocation action
        Elf32_Word  r_info;     // gives both the symbol table index with respect to which the relocation mus be made, and the type of relocation to apply
} Elf32_Rel;

typedef struct {
        Elf32_Addr  r_offset;
        Elf32_Word  r_info;
        Elf32_Sword r_addend;
} Elf32_Rela;

/* ----------------------------*/
/*       Program Header        */
/* ---------------- -----------*/

typedef struct {
        Elf32_Word p_type;	// tells what kind of segment this array element describes
        Elf32_Off  p_offset;	// gives the offset from the beginning of the file at which the first byte of the segment resides
        Elf32_Addr p_vaddr;	// gives the virtual address at which the first byte of the segment resides
        Elf32_Addr p_paddr;	// reserved for the segment's physical address
        Elf32_Word p_filesz;	// gives the number of bytes in the file image of the segment
        Elf32_Word p_memsz;	// gives the number of bytes in the memory image of the segment
        Elf32_Word p_flags;	// gives flags relevant to the segment
        Elf32_Word p_align;	//
} Elf32_Phdr;


#endif // ELF_STRUCTS_H_INCLUDED
