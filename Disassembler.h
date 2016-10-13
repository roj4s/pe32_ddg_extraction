//
// Created by rojas on 11/10/16.
//

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLER_H
#define ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLER_H

#include "DisassemblingInfo.h"
#include "CustomCFGImpl.h"
#include <memory>
#include <capstone/capstone.h>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

class Disassembler {

public:

    static std::string hex_from_int64(uint64_t addr);
    static bool is_jump_instruction(std::shared_ptr<cs_insn> instruction);
    static bool is_ret_instruction(std::shared_ptr<cs_insn> instruction);
    static DisassemblingInfo get_instructions_from_file(std::string file_addr);
    static std::shared_ptr<CustomCFGImpl> get_cfg(std::shared_ptr<DisassemblingInfo> info);

    static void add_basic_block(std::string entry_point, std::shared_ptr<std::unordered_set<std::string>> visited,
                                std::shared_ptr<CustomBasicBlockImpl> from, std::shared_ptr<std::queue<std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>>>> entry_points,
                                std::shared_ptr<DisassemblingInfo> info,
                                std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks,
                                std::shared_ptr<std::unordered_set<std::string>> non_existent_addresses,
                                std::shared_ptr<std::queue<std::string>> call_return_addresses, bool is_first_bb = true);
    
    static std::shared_ptr<std::unordered_set<std::string>> conditional_branch = std::make_shared<std::unordered_set<std::string>>({"jo", "jno", "jb", "jae", "jnae", "jc", "jnb", "jae", "jnc", "jz", "je", "jnz",
                                                                                                                                    "jne", "jbe", "jna", "jnbe", "ja", "js", "jns", "jp", "jpe", "jnp", "jpo",
                                                                                                                                    "jl", "jnge", "jnl", "jge", "jle", "jng", "jnle", "jg", "loop"});

    static std::shared_ptr<std::unordered_set<std::string>> function_call = std::make_shared<std::unordered_set<std::string>>({"call", "callf"});
    static std::shared_ptr<std::unordered_set<std::string>> unconditional_branch = std::make_shared<std::unordered_set<std::string>>({"jmp", "jmpf"});
    static std::shared_ptr<std::unordered_set<std::string>> return_instr = std::make_shared<std::unordered_set<std::string>>({"ret"});




};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLER_H
