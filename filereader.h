#ifndef FILEREADER_H
#define FILEREADER_H
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

typedef enum {
    // ECX: program pointer
    // [ECX]: current part of memory
    INCREMENT = 0, // inc [ECX]
    DECREMENT, // dec [ECX]
    LEFT, // dec ECX
    RIGHT, // inc ECX
    BEGIN_LOOP, // Label
    END_LOOP, // jnz BEGIN_LOOP
    INPUT, // getch
    OUTPUT //putc
} bfOp;

std::vector<bfOp> read_bf_file(std::string path);


#endif