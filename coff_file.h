#ifndef COFF_FILE_H
#define COFF_FILE_H
#include "filereader.h"

typedef struct {
	unsigned short 	f_magic;	/* Magic number */	
	unsigned short 	f_nscns;	/* Number of Sections */
	long 		    f_timdat;	/* Time & date stamp */
	long 	    	f_symptr;	/* File pointer to Symbol Table */
	long 	     	f_nsyms;	/* Number of Symbols */
	unsigned short 	f_opthdr;	/* sizeof(Optional Header) */
	unsigned short 	f_flags;	/* Flags */
} header_t;

typedef struct {
    union{
	    char*		name;	/* Section Name */
        long long   name_num;
    } s_name;
    long		s_paddr;	/* Physical Address */
	long		s_vaddr;	/* Virtual Address */
	long		s_size;		/* Section Size in Bytes */
	long		s_scnptr;	/* File offset to the Section data */
	long		s_relptr;	/* File offset to the Relocation table for this Section */
	long		s_lnnoptr;	/* File offset to the Line Number table for this Section */
	unsigned short	s_nreloc;	/* Number of Relocation table entries */
	unsigned short	s_nlnno;	/* Number of Line Number table entries */
	long		s_flags;	/* Flags for this section */
    std::vector<unsigned char> data;
} section_header_t;

struct str_offset{
    unsigned long e_zeroes;
    unsigned long e_offset;
};
typedef struct {
  union {
    char* e_name;
    struct str_offset e;
  } e;
  unsigned long e_value;
  short e_scnum;
  unsigned short e_type;
  unsigned char e_sclass;
  unsigned char e_numaux;
  char* aux;
} symbol;

struct relocation{
	long		r_vaddr;	/* Reference Address */
	long		r_symndx;	/* Symbol index */
	unsigned short	r_type;		/* Type of relocation */
};

class RelocationTable
{
    public:
    RelocationTable(bool isNull);
    void add_relocation(long r_vaddr, long r_symndx, unsigned short r_type);
    std::vector<struct relocation> relocations;
    std::vector<char> get_data();
    int get_size();
    bool isNull;
};

class CoffFile {
    public:
    CoffFile();
    void add_section(char name[8], int32_t flags, RelocationTable rt, std::vector<unsigned char> data);
    void add_symbol(std::string name, unsigned long value, short scnum, 
                        unsigned short type, unsigned char sclass, 
                        unsigned char e_numaux, std::string aux);
    void compile();
    std::string get_compiled();
    private:
    void coff_pbn(int32_t number, char size);// size: 1,2, or 4
    header_t head;
    std::vector<section_header_t> sections;
    std::vector<symbol> symbols;
    std::vector<RelocationTable> rts;
    std::vector<char> data;
    std::vector<char> string_table;
};

#endif