#ifndef ANALYZER_H
#define ANALYZER_H
#include <iostream>
#include <string>
#include <vector>
#include "filereader.h"



typedef struct{
    int start_index;
    int end_index;
} loop_t;
typedef struct {
    unsigned char memory[30000];
    int pointer;
    int op_pointer;
    loop_t current_loop; // the current_loop we're in
} state_t;

class BfFile{
    public:
    std::string raw;
    std::vector<bfOp> operations;
    std::vector<loop_t> loops;
    BfFile(std::string path);
};


#endif