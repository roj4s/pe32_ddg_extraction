#include <iostream>
#include "Disassembler.h"
#include "Aux.h"
#include <fstream>
#include <stdio.h>
#include <inttypes.h>
typedef unsigned char BYTE;

int main(void) {


//    std::stringstream converter;
//    std::istringstream ss( "13 00 0A 1B CA 00" );
//    std::vector<BYTE> bytes;
//
//    std::string word;
//    while( ss >> word )
//    {
//        BYTE temp;
//        converter << std::hex << word;
//        converter >> temp;
//        bytes.push_back( temp );
//    }
//
    std::string f  = "/tmp/f";
//
//
    std::shared_ptr<std::vector<cs_insn>> ins = Disassembler::get_instructions_from_file(f);
//    std::ofstream myfile;
//    myfile.open("/tmp/fromcpp_f");

//    const char * CODE_str = "\x55\x48\x8b\x05\xb8\x13\x00\x00";
//    std::string code = "55488b05b8130000";
//    char * CODE_str = new char[10000];
//    Aux::hex2bin(code.c_str(), CODE_str);
//    std::cout << CODE_str << std:: endl;
//    std::cout << code.size() << std::endl;
//    std::cout << sizeof(CODE_str) -1<< std::endl;

    FILE * file = fopen("/tmp/fromcpp_f", "w");

    for(std::vector<cs_insn>::iterator it = ins->begin(); it != ins->end(); ++it)
        fprintf(file,"0x%" PRIx64 ": %s %s\n", it->address, it->mnemonic, it->op_str);
//

//    std::cout << std::hex << 16 << std::endl;

    return 0;
}


//#include <capstone/capstone.h>
//#include <iostream>
//#include <typeinfo>
//
//#define g_DisasmCode "\x55\x48\x8b\x05\xb8\x13\x00\x00"
//
//int main(int, char**)
//{
//    csh handle;
//    cs_insn *insn;
//    size_t count;
//
//    // Ignore the architecture and mode, they are correct i just changed
//    // the names
//    if(cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
//        return -1;
//    std::cout<< typeid(g_DisasmCode).name() << std::endl;
//    count = cs_disasm(handle, (uint8_t*)g_DisasmCode, sizeof(g_DisasmCode) - 1, 0x1000, 0, &insn);
//    if(count > 0)
//    {
//        size_t j;
//        for(j = 0; j < count; j++)
//        {
//            printf("%i:\t%s\t\t%s\n", insn[j].size, insn[j].mnemonic,
//                   insn[j].op_str);
//
//        }
//
//        cs_free(insn, count);
//
//    }
//    else
//        printf("ERROR: Failed to disassemble given code!\n");
//
//    cs_close(&handle);
//}