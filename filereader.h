#ifndef FILEREADER_H
#define FILEREADER_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef enum {
    INCREMENT = 0, DECREMENT, LEFT, RIGHT, BEGIN_LOOP, END_LOOP, INPUT, OUTPUT
} bfOp;

std::vector<bfOp> read_bf_file(std::string path);


#endif