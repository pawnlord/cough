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
        std::cout << lines[i] << std::endl;
    }
    return operations;
}