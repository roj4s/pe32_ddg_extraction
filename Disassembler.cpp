//
// Created by rojas on 11/10/16.
//

#include "Disassembler.h"
#include "Aux.h"

std::shared_ptr<DisassemblingInfo> Disassembler::dissasemble(std::string file_addr) {

    std::string file_content = Aux::get_string_from_file(file_addr);
    bool entry_point_saved = false;
    std::string entry_point = "";

    std::vector<char> bytes = Aux::HexToBytes(file_content);
    char * CODE_str = reinterpret_cast<char *>(bytes.data());
    const uint8_t * CODE = reinterpret_cast<const uint8_t*>(CODE_str);

    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> instructions = std::make_shared<std::vector<std::shared_ptr<X86Instruction>>>();
    std::shared_ptr<std::unordered_map<std::string, int>> ins_addr_map = std::make_shared<std::unordered_map<std::string, int>>();


    csh handle;
    cs_insn *insn;
    size_t count;



    if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK)
        throw("Error disassembling");

    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    count = cs_disasm(handle, CODE, bytes.size(), 0x401000, 0, &insn);
    if (count > 0) {
        size_t j;
        for (j = 0; j < count; j++) {

//            char buff[15];
//            snprintf(buff, sizeof(buff), "0x%" PRIX64, insn[j].address);
//            std::string addr = buff;

            std::string addr = Disassembler::hex_from_int64(insn[j].address);

            if(!entry_point_saved){
                entry_point = addr;
                entry_point_saved = true;
            }

            std::shared_ptr<std::unordered_set<std::string>> regs_read = std::make_shared<std::unordered_set<std::string>>();
            std::shared_ptr<std::unordered_set<std::string>> regs_writen = std::make_shared<std::unordered_set<std::string>>();
            std::shared_ptr<std::vector<uint8_t >> bytes = std::make_shared<std::vector<uint8_t >>();

            for(int i =0; i< sizeof(insn[j].bytes)/sizeof(insn[j].bytes[0]); i++)
                bytes->push_back(insn[j].bytes[i]);

            cs_detail *detail = insn[j].detail;
            if(int reg_count = detail->regs_read_count > 0){
                for(int i =0; i< reg_count; i++){
                    uint16_t  reg_id = detail->regs_read[i];
                    const char * reg_char = cs_reg_name(handle, reg_id);
                    std::string reg = reinterpret_cast<const char *>(reg_char);
                    regs_read->insert(reg);
                }
            }

            if(int reg_count =detail->regs_write_count > 0){

                    for (int i=0; i< reg_count; i++){
                        uint16_t reg_id = detail->regs_write[i];
                        const char * reg_char = cs_reg_name(handle, reg_id);
                        std::string reg = reinterpret_cast<const char *>(reg_char);
                        regs_writen->insert(reg);
                    }

            }

            std::shared_ptr<X86Instruction> ins = std::make_shared<X86Instruction>(j, insn[j].size, insn[j].mnemonic,
                                                                                   insn[j].op_str, addr, regs_read, regs_writen,
                                                                                   bytes);
            instructions->push_back(ins);
            ins_addr_map->insert({addr, j});

            }
        cs_free(insn, count);
        } else
       throw ("ERROR: Failed to disassemble given code!");
        cs_close(&handle);

    std::shared_ptr<DisassemblingInfo> info = std::make_shared<DisassemblingInfo>(ins_addr_map, instructions, entry_point);
    return info;
}

std::shared_ptr<CustomCFGImpl> Disassembler::get_cfg(std::shared_ptr<DisassemblingInfo> info) {

    std::shared_ptr<std::queue<std::pair<std::string,std::shared_ptr<CustomBasicBlockImpl>>>> entry_points = std::make_shared<std::queue<std::pair<std::string,std::shared_ptr<CustomBasicBlockImpl>>>>();
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks = std::make_shared<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>>();
    std::shared_ptr<std::unordered_set<std::string>> visited_set = std::make_shared<std::unordered_set<std::string>>();
    std::shared_ptr<std::unordered_set<std::string>> non_existent_addresses = std::make_shared<std::unordered_set<std::string>>();
    std::shared_ptr<std::queue<std::string>> call_return_addresses = std::make_shared<std::queue<std::string>>();

    std::string first_entry_point = info->getEntry_point();
//    visited_set->insert(first_entry_point);
    std::shared_ptr<CustomBasicBlockImpl> empty_from = std::make_shared<CustomBasicBlockImpl>(true);
    std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> first_entry_pair = std::make_pair(first_entry_point, empty_from);
    entry_points->push(first_entry_pair);
    bool is_first_block = true;

    while(!entry_points->empty()){

        std::string entry_point = entry_points->front().first;

        std::shared_ptr<CustomBasicBlockImpl> from = entry_points->front().second;
        entry_points->pop();

        Disassembler::add_basic_block(entry_point, visited_set, from, entry_points, info, basic_blocks,
                                      non_existent_addresses, call_return_addresses, is_first_block);

        is_first_block = false;

    }
    std::shared_ptr<std::unordered_set<std::string>> cra = std::make_shared<std::unordered_set<std::string>>();

    while(!call_return_addresses->empty()){

        cra->insert(call_return_addresses->front());
        call_return_addresses->pop();

    }


    std::shared_ptr<CustomCFGImpl> cfg = std::make_shared<CustomCFGImpl>(basic_blocks, first_entry_point, visited_set, non_existent_addresses, cra);

    return cfg;

}

void Disassembler::add_basic_block(std::string entry_point, std::shared_ptr<std::unordered_set<std::string>> visited,
                                   std::shared_ptr<CustomBasicBlockImpl> from, std::shared_ptr<std::queue<std::pair<std::string,std::shared_ptr<CustomBasicBlockImpl>>>> entry_points,
                                   std::shared_ptr<DisassemblingInfo> info,
                                   std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks,
                                   std::shared_ptr<std::unordered_set<std::string>> non_existent_addresses,
                                   std::shared_ptr<std::queue<std::string>> call_return_addresses, bool is_first_block) {

    if(visited->find(entry_point) == visited->end()){

        visited->insert(entry_point);
        std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> instructions = std::make_shared<std::vector<std::shared_ptr<X86Instruction>>>();
        std::shared_ptr<std::unordered_map<std::string, int>> ins_map = std::make_shared<std::unordered_map<std::string, int>>();

        std::shared_ptr<std::unordered_map<std::string,int>> ins_addr_map = info->getIns_addr_map();
        std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> ins_vector = info->getIns_vector();

        if(ins_addr_map->find(entry_point) == ins_addr_map->end())
        {
            non_existent_addresses->insert(entry_point);

            if(!is_first_block){

                std::shared_ptr<CustomBasicBlockImpl> dead_end_bb = std::make_shared<CustomBasicBlockImpl>(true);
                from->add_sucessor(dead_end_bb);
                basic_blocks->insert({entry_point, dead_end_bb});

            }

        }
        else{

            bool in_basic_block = true;
            int index_of_entry_point_instruction = ins_addr_map->at(entry_point);
            std::shared_ptr<X86Instruction> instruction;
            int ins_map_index = 0;

            while(in_basic_block && index_of_entry_point_instruction != ins_vector->size()){

                instruction = ins_vector->at(index_of_entry_point_instruction);
                std::string address = instruction->getAddress();
                std::string mnemonic = instruction->getMnemonic();

                instructions->push_back(instruction);
                ins_map->insert({address, ins_map_index});

                if(Disassembler::is_branch_instruction(mnemonic)) {

                    in_basic_block = false;
                    std::shared_ptr<CustomBasicBlockImpl> this_bb = std::make_shared<CustomBasicBlockImpl>(instructions,ins_map, entry_point);

                    std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> ep_pair = std::make_pair(instruction->getOp_str(), this_bb);

                    if((Disassembler::unconditional_branch.find(mnemonic)
                       != Disassembler::unconditional_branch.end()) || (Disassembler::function_call.find(mnemonic)
                                                                       != Disassembler::function_call.end())){

                        entry_points->push(ep_pair);

                    }
                    else if(Disassembler::conditional_branch.find(mnemonic)
                       != Disassembler::conditional_branch.end()) {

                        int next_ins_index = index_of_entry_point_instruction + 1;

                        if(next_ins_index != ins_vector->size()){

                            std::string next_addr = ins_vector->at(next_ins_index)->getAddress();
                            std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> ep_pair_b = std::make_pair(next_addr, this_bb);

                            entry_points->push(ep_pair);
                            entry_points->push(ep_pair_b);


                        }

                    }
                    else if(Disassembler::is_ret_instruction(mnemonic)){

                        if(!call_return_addresses->empty()){

                            std::string addr_of_return = call_return_addresses->front();
                            std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> ret_pair = std::make_pair(addr_of_return, this_bb);
                            entry_points->push(ret_pair);

                        }
                        else{

                            std::shared_ptr<CustomBasicBlockImpl> dead_end_bb = std::make_shared<CustomBasicBlockImpl>(true);
                            this_bb->add_sucessor(dead_end_bb);

                        }
                    }

                    basic_blocks->insert({entry_point, this_bb});
                    if(!is_first_block){
                        from->add_sucessor(this_bb);
                        this_bb->add_predecessor(from);
                    }

                }
                index_of_entry_point_instruction ++;
                ins_map_index++;
            }

        }
    }
    else
    if(!is_first_block){

        std::shared_ptr<CustomBasicBlockImpl> to = basic_blocks->at(entry_point);
        from->add_sucessor(to);
        to->add_predecessor(from);

    }

}

bool Disassembler::is_branch_instruction(std::string mnemonic) {

    return (Disassembler::unconditional_branch.find(mnemonic)
            != Disassembler::unconditional_branch.end()) || (Disassembler::conditional_branch.find(mnemonic)
                                                              != Disassembler::conditional_branch.end()) || (Disassembler::return_instr.find(mnemonic)
                                                                                                              != Disassembler::return_instr.end()) ||
            (Disassembler::function_call.find(mnemonic) != Disassembler::function_call.end());

}
bool Disassembler::is_ret_instruction(std::string mnemonic) {

    return (Disassembler::return_instr.find(mnemonic)
            != Disassembler::return_instr.end());

}

std::string Disassembler::hex_from_int64(uint64_t addr) {

    char buff[15];
    snprintf(buff, sizeof(buff), "0x%" PRIX64, addr);
    std::string address_str = buff;
    return address_str;

}

std::string Disassembler::get_str_from_ins(std::shared_ptr<cs_insn> ins) {

    std::string str = Disassembler::hex_from_int64(ins->address) + ": " + ins->mnemonic + " " + ins->op_str;
    return str;
}

const std::unordered_set<std::string> Disassembler::conditional_branch = std::unordered_set<std::string>({"jo", "jno", "jb", "jae", "jnae", "jc",
"jnb", "jae", "jnc", "jz", "je", "jnz", "jne", "jbe", "jna", "jnbe", "ja", "js", "jns", "jp", "jpe", "jnp", "jpo","jl",
"jnge", "jnl", "jge", "jle", "jng", "jnle", "jg", "loop"});

const std::unordered_set<std::string> Disassembler::function_call = {"call", "callf"};
const std::unordered_set<std::string> Disassembler::unconditional_branch = {"jmp", "jmpf"};
const std::unordered_set<std::string> Disassembler::return_instr = {"ret"};
//

std::shared_ptr<DFG> Disassembler::get_dfg(std::shared_ptr<CustomCFGImpl> cfg) {

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>>> reach_outs = std::make_shared<std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>>>();
    std::shared_ptr<std::queue<std::pair<std::string,std::shared_ptr<CustomBasicBlockImpl>>>> bb_q = std::make_shared<std::queue<std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>>>>();
    std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>> bbs = cfg->getBasic_blocks();
    std::unordered_map<std::string, int> block_visited_times;


    for(std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>::iterator bb_it = bbs->begin(); bb_it!=bbs->end(); ++bb_it)
        bb_q->push(*bb_it);

    while(!bb_q->empty()){

        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> current_reach_out = std::make_shared<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>();
        std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> node = bb_q->front();
        bb_q->pop();

        block_visited_times[node.first] = block_visited_times[node.first]+1 ;

        std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> pred = node.second->getPredecessors();

        // Joining predecessors reach out
        if(pred->size() > 0){
            for(std::vector<std::shared_ptr<CustomBasicBlockImpl>>::iterator pred_it = pred->begin(); pred_it != pred->end(); ++pred_it){
                std::unordered_map<std::string,std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>>::iterator this_pred_defs = reach_outs->find((*pred_it)->getEntry_point());
                if(this_pred_defs != reach_outs->end()){
                    for(std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator regs_def_it = this_pred_defs->second->begin(); regs_def_it != this_pred_defs->second->end(); ++regs_def_it){
                        std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator curro_this_reg_addrs = current_reach_out->find(regs_def_it->first);

                        if(curro_this_reg_addrs != current_reach_out->end())
                            curro_this_reg_addrs->second->insert(regs_def_it->second->begin(), regs_def_it->second->end());
                        else
                        {
                            std::shared_ptr<std::unordered_set<std::string>> curroreg_addrs = std::make_shared<std::unordered_set<std::string>>();
                            curroreg_addrs->insert(regs_def_it->second->begin(), regs_def_it->second->end());
                            current_reach_out->insert({regs_def_it->first, curroreg_addrs});
                        }
                    }
                }
            }
        }

        // Processing instruction by instruction of this blog, adding edges and updating reach out
        for(std::vector<std::shared_ptr<X86Instruction>>::iterator ins_it = node.second->getInstructions()->begin(); ins_it != node.second->getInstructions()->end(); ++ins_it){



        }


    }

}
