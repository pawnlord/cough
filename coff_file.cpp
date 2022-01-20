#include "coff_file.h"
#include <time.h>
#include <iomanip>
#include <stdlib.h>


CoffFile::CoffFile(){
    head.f_magic = 0x014c;
    head.f_nscns = 0;
    head.f_timdat = time(NULL);
    head.f_symptr = 20;
    head.f_nsyms = 0;
    head.f_opthdr = 0;
    head.f_flags = 0;
    for(int i = 0; i < 4; i++){
        string_table.push_back(0); // init string table size
    }
    string_table_sz = 4;
}


void CoffFile::add_section(char name[8], int32_t flags, RelocationTable rt, std::vector<unsigned char> data){
    head.f_symptr+=40;
    section_header_t sh;
    if((flags & 0xFF) != 0x80) {
        sh = {name, 0, 0, data.size(), head.f_symptr, head.f_symptr+data.size(), 0, rt.relocations.size(), 0, flags, data};
        head.f_symptr+=data.size();
        head.f_symptr+=rt.get_size();
    } else {
        sh = {name, 0, 0, 0, 0, 0, 0, 0, 0, flags, data};
        bss_sections.push_back(sections.size()+1);
    }
    
    // After adding a new section, every section is offset by 40
    for(int i = 0; i < sections.size(); i++){
        if((sections[i].s_flags & 0xFF) != 0x80){
            sections[i].s_scnptr+=40;
            sections[i].s_relptr+=40;
        }
    }
    
    sections.push_back(sh);
    head.f_nscns += 1;
    rts.push_back(rt);
}

void CoffFile::coff_pbn(int32_t number, char size){
    push_back_number(this->data, number, size);
}

void CoffFile::add_symbol(std::string name, unsigned long value, short scnum, unsigned short type, unsigned char sclass, unsigned char numaux, std::string aux){
    char* full_aux;
    std::cout << name << std::endl;
    if(aux != ""){
        full_aux =  (char*)malloc(18);
        for(int i = 0; i < 18; i++){
            full_aux[i] = 0;
        }
        strcpy(full_aux, aux.c_str());
        std::cout << "full_aux" << full_aux <<std::endl;
    } 
    if(name.length() <= 8){
        char* final_name = (char*)malloc(8);
        for(int i = 0; i < 8; i++){
            if(i < name.length()){
                final_name[i] = name[i];
            } else{
                final_name[i] = 0;
            }
        }
        std::cout << final_name<< std::endl;
        symbol s = {final_name, value, scnum, type, sclass, numaux, full_aux};
        std::cout << "full_aux" << s.aux <<std::endl;
        symbols.push_back(s);
    } // We need the string table otherwise
    else{
        int offset = string_table.size();
        for(int i = 0; name[i] != 0; i++){
            string_table.push_back(name[i]);
        }
        string_table.push_back(0);
        string_table_sz += name.size() + 1;
        struct str_offset str_pointer = {0, offset};
        sym_name sn;
        sn.e = str_pointer;
        symbol s = {sn, value, scnum, type, sclass, numaux, full_aux};
        symbols.push_back(s);
    }
    for(int i = 0; i < bss_sections.size(); i++){
        if(scnum == bss_sections[i]){
            std::cout << "bss symbol: " << name << std::endl;
            sections[bss_sections[i]-1].s_size += 4;
        }
    }
    head.f_nsyms += 1 + (aux == ""?0:1);
}

void CoffFile::compile(){
    data.clear();
    // compile header
    coff_pbn(head.f_magic, 2);
    coff_pbn(head.f_nscns, 2);
	coff_pbn(head.f_timdat, 4);
	coff_pbn(head.f_symptr, 4);	
	coff_pbn(head.f_nsyms, 4);
	coff_pbn(head.f_opthdr, 2);	
	coff_pbn(head.f_flags, 2);
    std::cout << "header compiled" << std::endl;

    // compile section headers
    for(int i = 0; i < sections.size(); i++){
        std::cout << sections[i].s_name.name << std::endl;
        for(int j = 0; j < 8; j++){
            coff_pbn(sections[i].s_name.name[j], 1);
        }
        coff_pbn(sections[i].s_paddr, 4);
        coff_pbn(sections[i].s_vaddr, 4);
        coff_pbn(sections[i].s_size, 4);
        coff_pbn(sections[i].s_scnptr, 4);
        coff_pbn(sections[i].s_relptr, 4);
        coff_pbn(sections[i].s_lnnoptr, 4);
        coff_pbn(sections[i].s_nreloc, 2);
        coff_pbn(sections[i].s_nlnno, 2);
        coff_pbn(sections[i].s_flags, 4);
    }
    std::cout << "section headers compiled" << std::endl;
    std::vector<char> rdata;
    for(int i = 0; i < sections.size(); i++){
        data.insert(data.end(), sections[i].data.begin(), sections[i].data.end());
        rdata = rts[i].get_data();
        data.insert(data.end(), rdata.begin(), rdata.end()); // TODO: possible error
    }
    std::cout << "sections compiled" << std::endl;
    for(int i = 0; i < symbols.size(); i++){
        if(symbols[i].e.e.e_zeroes != 0){
            std::cout << symbols[i].e.e_name << std::endl;
            for(int j = 0; j < 8; j++){
                coff_pbn(symbols[i].e.e_name[j], 1);
            }
        } else {
            coff_pbn(symbols[i].e.e.e_zeroes, 4);
            coff_pbn(symbols[i].e.e.e_offset, 4);
        }
        coff_pbn(symbols[i].e_value, 4);
        coff_pbn(symbols[i].e_scnum, 2);
        coff_pbn(symbols[i].e_type, 2);
        coff_pbn(symbols[i].e_sclass, 1);
        coff_pbn(symbols[i].e_numaux, 1);
        if(symbols[i].e_numaux == 1){
            std::cout << std::hex << (int)symbols[i].aux[0] << std::endl;
            for(int j = 0; j < 18; j++){
                coff_pbn(symbols[i].aux[j], 1);
            }
        }
    }
    std::cout << "symbols compiled" << std::endl;
    
    std::cout << "size of str table: " << string_table_sz;
    // set string table size.
    for(int i = 0; i < 4; i++){
        string_table[i] = (string_table_sz>>(i*8))&0xFF;
    }

    data.insert(data.end(), string_table.begin(), string_table.end());
    std::cout << "string table added" << std::endl;


    std::cout << "Compiled. size of data: " << data.size();
}

std::string CoffFile::get_compiled(){
    std::string compiled = "";
    for(int i = 0; i < data.size(); i++){
        compiled += data[i];
    }
    return compiled;
}
