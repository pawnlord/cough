#include "filereader.h"

std::vector<bfOp> read_bf_file(std::string path){
    std::ifstream file;
    std::string line;
    std::vector<std::string> lines;
    file.open(path);
    std::string parsed_line = "";
    if(file.is_open()){
        while(std::getline(file, line)){
            parsed_line = "";
            for(int c = 0; line.c_str()[c] != 0; c++){
                if(line[c] == '#'){
                    break;
                }
                parsed_line += line[c];
            }
            lines.push_back(parsed_line);
        }
    }
    else{
        std::cerr << "[cerebellum: COMPILE ERROR] file " << path << "not found." << std::endl;
        exit(1);
    }
    std::vector<bfOp> operations;
    for(int i = 0; i < lines.size(); i++){
        for(int c = 0; lines[i].c_str()[0] == 0; i++){
            switch(lines[i][c]){
                case '+':
                    operations.push_back(INCREMENT);
                break;
                case '-':
                    operations.push_back(DECREMENT);
                break;
                case '>':
                    operations.push_back(RIGHT);
                break;
                case '<':
                    operations.push_back(RIGHT);
                break;
                case '.':
                    operations.push_back(OUTPUT);
                break;
                case ',':
                    operations.push_back(INPUT);
                break;
                case '[':
                    operations.push_back(BEGIN_LOOP);
                break;
                case ']':
                    operations.push_back(END_LOOP);
                break;
            }
        }
    }
    return operations;
}