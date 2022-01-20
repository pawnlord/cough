#include "coff_utils.h"

void push_back_number(std::vector<char>& ref, int32_t number, char size){
    int temp = number;
    for(int i = 0; i < size; i++){
        unsigned char byte = temp&0xff;
        ref.push_back(byte);
        temp>>=8; // remove last byte
    }
}
RelocationTable::RelocationTable(bool isNull){
    this->isNull = isNull;
}

void RelocationTable::add_relocation(long r_vaddr, long r_symndx, unsigned short r_type){
    relocations.push_back({r_vaddr, r_symndx, r_type});
}

int RelocationTable::get_size(){
    return relocations.size() * 10;
}

std::vector<char> RelocationTable::get_data(){
    if(isNull){
        return std::vector<char>();
    } else {
        std::vector<char> data;
        for(int r = 0; r < relocations.size(); r++){
           push_back_number(data, relocations[r].r_vaddr, 4); 
           push_back_number(data, relocations[r].r_symndx, 4);
           push_back_number(data, relocations[r].r_type, 2);
        }
        return data;
    }
}