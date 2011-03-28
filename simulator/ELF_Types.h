#ifndef ELF_TYPES_H_INCLUDED
#define ELF_TYPES_H_INCLUDED

/* ------------------------*/
/*    32-Bit Data Types    */
/* ------------------------*/
                                         // Size | Alignment | Purpose

typedef unsigned long Elf32_Addr;       //  4   |  4        | Unsigned program address
typedef unsigned  int Elf32_Half; 	//  2   |  2        | Unsigned medium integer
typedef unsigned long Elf32_Off;  	//  4   |  4        | Unsigned file offset
typedef   signed long Elf32_Sword;	//  4   |  4        | Signed large integer
typedef unsigned long Elf32_Word; 	//  4   |  4        | Unsigned large integer;
					//unsigned char; //  1   |  1        | Unsigned small integer;

/* -------------------------*/
/*    ELF Identification    */
/* -------------------------*/

const int EI_MAG0    =   0;  // File identification
const int EI_MAG1    =   1;  // File identification
const int EI_MAG2    =   2;  // File identification
const int EI_MAG3    =   3;  // File identification
const int EI_CLASS   =   4;  // File class             ELFCLASS32  for 32-bit Intel
const int EI_DATA    =   5;  // Data encoding          ELFDATA2LSB for 32-bit Intel
const int EI_VERSION =   6;  // File vesion
const int EI_PAD     =   7;  // Start of padding bytes

/* -----------------------------------*/
/*    Access bytes' values            */
/* -----------------------------------*/

// "magic numbers"
const int ELFMAG0    =   0x7f;   // e_ident[EI_MAG0]
const int ELFMAG1    =    'E';   // e_ident[EI_MAG1]
const int ELFMAG2    =    'L';   // e_ident[EI_MAG2]
const int ELFMAG3    =    'F';   // e_ident[EI_MAG3]

// file's class (capacity)
const int ELFCLASSNONE   =   0;  // Invalid class
const int ELFCLASS32     =   1;  // 32-bit objects
const int ELFCLASS64     =   2;  // 64-bit objects

// data encoding of the processor-specific data in the object file
const int ELFDATANONE    =   0;  // Invalid data encoding
const int ELFDATA2LSB    =   1;  // the least significant byte occupying the lowest address
const int ELFDATA2MSB    =   2;  // the most significany byte ocupying the lowest address

/* ------------------------*/
/*    Object File Types    */
/* ------------------------*/

const int ET_NONE    =      0;   // No file type
const int ET_REL     =      1;   // Relocatable file
const int ET_EXEC    =      2;   // Executable file
const int ET_DYN     =      3;   // Shared object file
const int ET_CORE    =      4;   // Core file
const int ET_LOPROC  = 0xff00;   // Processor-specific
const int ET_HIPROC  = 0xffff;   // Processor-specific

/* ------------------------------------------*/
/*    Architecture for an individual file    */
/* ------------------------------------------*/

const int EM_NONE    =   0;  // No machine
const int EM_M32     =   1;  // AT&Y WE 32100
const int EM_SPARC   =   2;  // SPARC
const int EM_386     =   3;  // Intel80386
const int EM_68K     =   4;  // Motorola 68000;
const int EM_88K     =   5;  // Motorola 88000;
const int EM_860     =   7;  // Intel 80860
const int MIPS       =   8;  // MIPS RS3000;

/* -------------------------*/
/*   Object File Version    */
/* --------------------- ---*/

const int EV_NONE    =   0;  // Invalid version
const int EV_CURRENT =   1;  // Current version

/* --------------------------*/
/*   Object File's Section   */
/* --------------------------*/

const int SHN_UNDEF      =        0; // Undefined value
const int SHN_LORESERVE  =   0xff00; // The lower bound of the range of reserved indexes
const int SHN_LOPROC     =   0xff00; // Values which are reserved for processor-specific semantics
const int SHN_HIPROC     =   0xff1f;
const int SHN_ABS        =   0xfff1; // Absolute values for the corresponding reference
const int SHN_COMMON     =   0xfff2; // Symbols defined relative to this section are common symbols
const int SHN_HIRESERVE  =   0xffff; // The upper bound of the range of reserved indexes

/* ------------------*/
/*   Section Types   */
/* ------------------*/

const int SHT_NULL       =            0; // Inactive section header
const int SHT_PROGBITS   =            1; // Information defined by the program
const int SHT_SYMTAB     =            2; // A symbol table
const int SHT_STRTAB     =            3; // A string table
const int SHT_RELA       =            4; // Relocation entries with explicit depends
const int SHT_HASH       =            5; // A symbol hash table
const int SHT_DYNAMIC    =            6; // Information for dynamic linking
const int SHT_NOTE       =            7; // Information that marks the file in some way
const int SHT_NOBITS     =            8; // Occupies no space in the file
const int SHT_REL        =            9; // Relocation entries without explicit addends
const int SHT_SHLIB      =           10; // Has unspecified semantics
const int SHT_DYNSYM     =           11;
const int SHT_LOPROC     =   0x70000000; // Values reserved for processor-specific semantics
const int SHT_HIPROC     =   0x7fffffff;
const int SHT_LOUSER     =   0x80000000;
const int SHT_HIUSER     =   0xffffffff;

/* ----------------------------*/
/*   Section Attribute Flags   */
/* ----------------------------*/

const int SHF_WRITE      =           0x1; // Data that should be writable during process execution
const int SHF_ALLOC      =           0x2; // Occupies memory during process execution
const int SHF_EXECINSTR  =           0x4; // Contains executable machine instructions
const int SHF_MASKPROC   =    0xf0000000; // All bits included in this mask are reserved for processor-specific semantics

/* ----------------------------*/
/*   Symbol Binding            */
/* ----------------------------*/

const int STB_LOCAL      =   0;  // Local symbols are not visible outside the object file containing their definition
const int STB_GLOBAL     =   1;  // Global symbols are visible to all object files being combined
const int STB_WEAK       =   2;  // Weak symbols resemble global symbols, but their definitions have lower precedence
const int STB_LOPROC     =   13; // Values in this inclusive range are reserved for processor-specific semantics
const int STB_HIPROC     =   15;

/* ----------------------------*/
/*     Symbol Types            */
/* ----------------------------*/

const int STT_NOTYPE     =   0;  // The symbol's type is not specified
const int STT_OBJECT     =   1;  // The symbol is associated with a data object, such as variable, an array, etc.
const int STT_FUNC       =   2;  // The symbol is associated with a function or other executable code
const int STT_SECTION    =   3;  // The symbol is associated with a section
const int STT_FILE       =   4;  // Conventionally, the symbol's name gives the name of the source file associated with the object file
const int STT_LOPROC     =   13; // Values in this inclusive range are reserved for processor-specific semantics
const int STT_HIPROC     =   15;

/* --------------------------------*/
/*         Relocation Types        */
/* --------------------------------*/

const int R_386_NONE     =   0;
const int R_386_32       =   1;
const int R_386_PC32     =   2;
const int R_386_GOT32    =   3;  // computes the distance from the base of the global offset table to the symbol's global offset table entry
const int R_386_PLT32    =   4;  // computes the address of the symbol's procedure linkage table entry and instructs the link editor to build a procedure linkage table
const int R_386_COPY     =   5;  // the link editor creates this relocation type for dynamic linking
const int R_386_GLOB_DAT =   6;  // is used to set global offset table entry to the address of the specified symbol
const int R_386_JMP_SLOT =   7;  // the link editor creates this relocation type for dynamic linking
const int R_386_RELATIVE =   8;  // the link editor creates this relocation type for dynamic linking
const int R_386_GOTOFF   =   9;  // computes the difference between a symbol's value and the address of the global offset table
const int R_386_GOTPC    =  10;


/* --------------------------*/
/*       Segment Types       */
/* --------------------------*/

const int PT_NULL	= 			0;	// element is unused
const int PT_LOAD	= 			1;	// specifies a loadable segment
const int PT_DYNAMIC	=			2;	// specifies dynamic linking information
const int PT_INTERP	=			3;	// specifies the location and size of a null-terminated path name
const int PT_NOTE	=			4;	// specifies the location and size of auxiliary information
const int PT_SHLIB	=			5;	// reserved
const int PT_PHDR	=			6;
const int PT_LOPROC	=  0x70000000;
const int PT_HIPROC	=  0x7fffffff;


#endif // ELF_TYPES_H_INCLUDED
