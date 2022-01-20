#include "filereader.h"
#include "coff_file.h"
#include <fstream>

int main(){
    read_bf_file("test_file.bf");
    CoffFile cf;

    RelocationTable null_rt(true);
    cf.add_section(".data\0\0\0", 0xC0300040, null_rt, std::vector<unsigned char>({
        0x49, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0D, 0x0A, 0x00
    }));
    cf.add_section(".bss\0\0\0\0", 0xC0300080, null_rt, std::vector<unsigned char>({}));
    RelocationTable text_rt(false);
    text_rt.add_relocation(3, 0xC, 0x14);
    text_rt.add_relocation(0xA, 0x4, 0x6);
    text_rt.add_relocation(0x11, 0x2, 0x6);
    text_rt.add_relocation(0x17, 0xD, 0x14);
    text_rt.add_relocation(0x1E, 0xB, 0x14);
    std::cout << "text relocations made" << std::endl;
 
    cf.add_section(".text\0\0\0", 0x60500020, text_rt, std::vector<unsigned char>({
        0x6A, 0xF5, 0xE8, 0x00, 0x00, 0x00, 0x00, // call GetStdHandle (7) 
        0x6A, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x0F, 0x68, 0x00, 0x00, 0x00, 0x00, 0x50, 0xE8, 0x00, 0x00, 0x00, 0x00, // Print "Hello World!" (20)
        0x6A, 0x00, 0x68, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x0F, 0x68, 0x00, 0x00, 0x00, 0x00, 0x50, 0xE8, 0x00, 0x00, 0x00, 0x00, // Print "Hello World!" (20)
        0x6A, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00 // Exit (7)
    }));
    std::cout << "sections made" << std::endl;
    cf.add_symbol(".file", 0, -2, 0, 0x67, 1, ".\\test.asm");
    cf.add_symbol(".data", 0, 1, 0, 0x3, 1, "\017\0");
    cf.add_symbol(".bss", 0, 2, 0, 0x3, 1, "\04\0");
    cf.add_symbol(".text", 0, 3, 0, 0x3, 1, "\01\0");
    cf.add_symbol(".absolut", 0, -1, 0, 0x3, 0, "");
    cf.add_symbol("STD_OUTPUT_HANDLE\0", 0xFFFFFFF5, -1, 0, 0x3, 0, "");
    cf.add_symbol("NULL", 0, -1, 0, 0x3, 0, "");
    cf.add_symbol("ExitProcess", 0, 0, 0, 0x2, 0, "");
    cf.add_symbol("GetStdHandle", 0, 0, 0, 0x2, 0, "");
    cf.add_symbol("WriteConsoleA", 0, 0, 0, 0x2, 0, "");
    cf.add_symbol("msg", 0, 1, 0, 0x3, 0, "");
    cf.add_symbol("msg.len", 0xF, -1, 0, 0x3, 0, "");
    cf.add_symbol("dummy", 0, 2, 0, 0x3, 0, "");
    cf.add_symbol("main", 0, 3, 0, 0x2, 0, "");
    cf.add_symbol("@feat.00", 1, -1, 0, 0x3, 0, "");
    std::cout << "symbols made" << std::endl;
    cf.compile();
    std::cout << "compiled" << std::endl;

    std::string data = cf.get_compiled();
    std::ofstream out ("test_build.obj", std::ios::binary);
    out.write(data.c_str(), data.size()+1);

    std::cout << "tests complete" << std::endl;
}