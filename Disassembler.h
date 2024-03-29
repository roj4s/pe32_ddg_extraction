//
// Created by rojas on 11/10/16.
//




#ifndef ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLER_H
#define ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLER_H

#include "DisassemblingInfo.h"
#include "DFG.h"
#include "CustomCFGImpl.h"
#include <memory>
#include <capstone/capstone.h>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

class Disassembler {

public:

    static std::string get_str_from_ins(std::shared_ptr<cs_insn>);
    static std::string hex_from_int64(uint64_t addr);
    static bool is_branch_instruction(std::string mnemonic);
    static bool is_ret_instruction(std::string mnemonic);


    static void write_asm_to_file(const std::string &file_addr, std::shared_ptr<DisassemblingInfo> inf);

    static std::shared_ptr<CustomCFGImpl> get_cfg(std::shared_ptr<DisassemblingInfo> info);
    static std::shared_ptr<boost::DFGraph> get_dfg(std::shared_ptr<CustomCFGImpl> cfg);

    static bool reach_outs_notequal(
            std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> r1,
            std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> r2);

    static std::string inst_uses_and_defs_from_cfg(std::shared_ptr<CustomCFGImpl> cfg);


    static std::string reach_out_as_string(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> r1);


    static void add_basic_block(std::string entry_point, std::shared_ptr<std::unordered_set<std::string>> visited,
                                std::shared_ptr<CustomBasicBlockImpl> from, std::shared_ptr<std::queue<std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>>>> entry_points,
                                std::shared_ptr<DisassemblingInfo> info,
                                std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks,
                                std::shared_ptr<std::unordered_set<std::string>> non_existent_addresses,
                                std::shared_ptr<std::queue<std::string>> call_return_addresses, bool is_first_bb = true);
    
    static const std::unordered_set<std::string> conditional_branch;
    static const std::unordered_set<std::string> function_call;
    static const std::unordered_set<std::string> unconditional_branch;
    static const std::unordered_set<std::string> return_instr;
    static std::pair<std::string, std::string> getEntryPointAndAssemblyFromCustomFile(std::string input_file);
    static std::shared_ptr<DisassemblingInfo> dissasemble
            (std::string hex_dump, std::string md5, const std::string &entry_point);
    static std::shared_ptr<DisassemblingInfo> dissasemble_from_hexdump_in_file
            (std::string file_addr, std::string md5, const std::string &entry_point);

};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLER_H
