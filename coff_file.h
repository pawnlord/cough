#ifndef COFF_FILE_H
#define COFF_FILE_H
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "coff_utils.h"


class CoffFile {
    public:
    CoffFile();
    CoffFile(std::string file_name);
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
    std::vector<int> bss_sections;
    int string_table_sz;
};

#endif