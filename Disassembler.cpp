//
// Created by rojas on 11/10/16.
//

#include "Disassembler.h"
#include "Aux.h"


std::shared_ptr<DisassemblingInfo> Disassembler::dissasemble_from_hexdump_in_file(std::string file_addr,
                                                                                  std::string md5,
                                                                                  const std::string &hex_entry_point) {

    std::string file_content = Aux::get_string_from_file(file_addr);
    return Disassembler::dissasemble(file_content, md5, hex_entry_point);
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


    std::shared_ptr<CustomCFGImpl> cfg = std::make_shared<CustomCFGImpl>(basic_blocks, first_entry_point, visited_set,
                                                                         non_existent_addresses, cra, info->getMd5());

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
                std::string addr = instruction->getAddress();
                if(basic_blocks->find(instruction->getAddress()) != basic_blocks->end()){
                    std::shared_ptr<CustomBasicBlockImpl> this_bb = std::make_shared<CustomBasicBlockImpl>(instructions,ins_map, entry_point);
                    from->add_sucessor(this_bb);
                    this_bb->add_predecessor(from);
                    basic_blocks->insert({entry_point, this_bb});

                    break;

                }

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


std::shared_ptr<boost::DFGraph> Disassembler::get_dfg(std::shared_ptr<CustomCFGImpl> cfg) {

    boost::DFGraph g = boost::DFGraph(0);
//    boost::DFGMd5 md5Prop = boost::get(boost::md5_t(), g);
//    md5Prop = cfg->getMd5();
    boost::DFGVMnemonicMap mnemonicMap = boost::get(boost::mnemonic_t(), g);
    boost::DFGVOpstrMap opstrMap = boost::get(boost::op_str_t(), g);
    boost::DFGVAddressMap addressMap = boost::get(boost::address_hex_str_t(), g);
    boost::DFGVInststrMap inststrMap = boost::get(boost::ins_str_t(), g);
    boost::DFGVRegUsedMap regUsedMap = boost::get(boost::reg_used_t(), g);

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>>> reach_outs = std::make_shared<std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>>>();
    std::shared_ptr<std::queue<std::pair<std::string,std::shared_ptr<CustomBasicBlockImpl>>>> bb_q = std::make_shared<std::queue<std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>>>>();
    std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>> bbs = cfg->getBasic_blocks();
    std::unordered_map<std::string, int> block_visited_times;
    std::shared_ptr<std::unordered_map<std::string, boost::DFGVertex>> vertexes_mapping = std::make_shared<std::unordered_map<std::string, boost::DFGVertex>>();
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> edges_mapping = std::make_shared<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>();

//    std::shared_ptr<std::unordered_map<std::string, std::string>> mnemonic_prop_mapping = std::make_shared<std::unordered_map<std::string, std::string>>();

    for(std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>::iterator bb_it = bbs->begin();
        bb_it!=bbs->end(); ++bb_it)
        bb_q->push(*bb_it);

    while(!bb_q->empty()){


        std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> node = bb_q->front();
        bb_q->pop();
        if(node.second->getEntry_point() == "terminal") continue;

        std::string addr_of_this_block = node.first;

        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> current_reach_out = std::make_shared<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>();
        std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>>::iterator this_block_old_reach_out_it = reach_outs->find(node.first);
        std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> this_node_insts = node.second->getInstructions();
//        std::shared_ptr<std::unordered_map<std::string, int>> this_node_ins_index_mapping = node.second->getIns_map();


        block_visited_times[node.first] = block_visited_times[node.first]+1 ;
        int vt = block_visited_times[node.first];

        std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> pred = node.second->getPredecessors();

        // Joining predecessors reach out
        if(pred->size() > 0){

            for(std::vector<std::shared_ptr<CustomBasicBlockImpl>>::iterator pred_it = pred->begin(); pred_it != pred->end(); ++pred_it){
                std::unordered_map<std::string,std::shared_ptr<std::unordered_map<std::string,
                        std::shared_ptr<std::unordered_set<std::string>>>>>::iterator this_pred_defs = reach_outs->find((*pred_it)->getEntry_point());

                if(this_pred_defs != reach_outs->end()){

                    for(std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator regs_def_it = this_pred_defs->second->begin(); regs_def_it != this_pred_defs->second->end(); ++regs_def_it){
                        std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator curro_this_reg_addrs = current_reach_out->find(regs_def_it->first);

                        if(curro_this_reg_addrs != current_reach_out->end()){

                            curro_this_reg_addrs->second->insert(regs_def_it->second->begin(), regs_def_it->second->end());

                        }
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

        std::string current_reach_out_str = Disassembler::reach_out_as_string(current_reach_out);

        // Processing instruction by instruction of this block, adding edges and updating reach out
        for(std::vector<std::shared_ptr<X86Instruction>>::iterator ins_it = this_node_insts->begin();
            ins_it != this_node_insts->end(); ++ins_it){

            boost::DFGVertex this_inst_vertex_desc;

            //Adding node if not exist in graph.
            std::string ins_addr = (*ins_it)->getAddress();

            std::unordered_map<std::string, boost::DFGVertex>::iterator b_v_t_i = vertexes_mapping->find(ins_addr);
            if(b_v_t_i != vertexes_mapping->end()){
                block_visited_times[ins_addr] += 1;
                this_inst_vertex_desc = b_v_t_i->second;

            }
            else{

                block_visited_times[ins_addr] = 1;
                this_inst_vertex_desc = boost::add_vertex(g);
                vertexes_mapping->insert({ins_addr,this_inst_vertex_desc});

                std::string mnemonic = (*ins_it)->getMnemonic();
                std::string op_str = (*ins_it)->getOp_str();
                std::string insstr = ins_addr + ": " + mnemonic + " " + op_str;

                mnemonicMap[this_inst_vertex_desc] = mnemonic;
                opstrMap[this_inst_vertex_desc] = op_str;
                addressMap[this_inst_vertex_desc] = ins_addr;
                inststrMap[this_inst_vertex_desc] = insstr;

                std::shared_ptr<std::unordered_set<std::string>> em = std::make_shared<std::unordered_set<std::string>>();
                edges_mapping->insert({ins_addr, em});

            }


            std::shared_ptr<std::unordered_set<std::string>> regs_read = (*ins_it)->getRegs_read();
            std::shared_ptr<std::unordered_set<std::string>> target_addrss = edges_mapping->at(ins_addr);

            //Adding edges from this node to the modifiers of the regs it uses if the edges don't exists.
            for(std::unordered_set<std::string>::iterator read_reg_it = regs_read->begin(); read_reg_it != regs_read->end(); ++read_reg_it){
                std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator curro_this_reg_addrs = current_reach_out->find((*read_reg_it));

                std::string reg = *read_reg_it;

                if(curro_this_reg_addrs != current_reach_out->end()){ //If have modifiers to the addresses uses by this ins.

                    current_reach_out_str = Disassembler::reach_out_as_string(current_reach_out);

                    for(std::unordered_set<std::string>::iterator addrs_it = curro_this_reg_addrs->second->begin();
                        addrs_it != curro_this_reg_addrs->second->end(); ++addrs_it){

                        bool not_exists = target_addrss->insert(*addrs_it).second;

                        if(not_exists){
                            boost::DFGVertex target = vertexes_mapping->at(*addrs_it);
                            boost::DFGEdge e = boost::add_edge(this_inst_vertex_desc, target, g).first;
                            regUsedMap[e] = reg;
                        }
                    }
                }
            }

            //Updating current reach out based on the regs this instruction modifies.
            std::shared_ptr<std::unordered_set<std::string>> regs_written = (*ins_it)->getRegs_writen();

            for(std::unordered_set<std::string>::iterator reg_writen_it = regs_written->begin();
                reg_writen_it != regs_written->end(); ++reg_writen_it){

                std::shared_ptr<std::unordered_set<std::string>> box_to_this_addr_only = std::make_shared<std::unordered_set<std::string>>();
                box_to_this_addr_only->insert(ins_addr);
                bool updated = current_reach_out->insert({*reg_writen_it, box_to_this_addr_only}).second;
                if(!updated)
                    current_reach_out->at(*reg_writen_it) = box_to_this_addr_only;
            }

            current_reach_out_str = Disassembler::reach_out_as_string(current_reach_out);

        }

        current_reach_out_str = Disassembler::reach_out_as_string(current_reach_out);
        if(this_block_old_reach_out_it != reach_outs->end())
        std::string old_reach_out_str = Disassembler::reach_out_as_string(this_block_old_reach_out_it->second);

        // Add sucessors if not old reach out, or current reach out different from old reach out.
        bool not_equal;
       if((this_block_old_reach_out_it == reach_outs->end()) ||(not_equal = Disassembler::reach_outs_notequal(current_reach_out, this_block_old_reach_out_it->second)))
       {

           bool updated = reach_outs->insert({node.first, current_reach_out}).second;

           if(!updated)
            reach_outs->at(node.first) = current_reach_out;

           std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> sucessors = node.second->getSucessors();
           for(std::vector<std::shared_ptr<CustomBasicBlockImpl>>::iterator succ_it = sucessors->begin(); succ_it != sucessors->end(); ++succ_it){
               std::string ep = (*succ_it)->getEntry_point();

               bb_q->push(std::make_pair((*succ_it)->getEntry_point(), (*succ_it)));
           }


       }

    }

    std::shared_ptr<boost::DFGraph> graph = std::make_shared<boost::DFGraph>(g);

    return graph;
}

bool Disassembler::reach_outs_notequal(
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> r1,
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> r2) {

    int equal_regs = 0;

    for(std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator r1_it; r1_it!= r1->end(); ++r1_it){
        for(std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator r2_it; r2_it!= r2->end(); ++r2_it)
        {
            if(r1_it->first == r2_it->first){
                equal_regs +=1;
                if(r1_it->second->size() != r2_it->second->size())
                    return true;

                for(std::unordered_set<std::string>::iterator addr_1_it = r1_it->second->begin(); addr_1_it != r1_it->second->end(); ++addr_1_it)
                    if(r2_it->second->find(*addr_1_it) == r2_it->second->end())
                        return true;
            }
        }
    }

    return equal_regs == 0;
}

std::string Disassembler::reach_out_as_string(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>> r1) {

    std::string to_ret ="";

    for(std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>::iterator r1_it = r1->begin(); r1_it!= r1->end(); ++r1_it){
        to_ret += r1_it->first + ":\n";

        for(std::unordered_set<std::string>::iterator addr_it = r1_it->second->begin(); addr_it != r1_it->second->end();
            ++addr_it)
            to_ret += "\t" + *addr_it + "\n";


    }

    return to_ret;

}

std::string Disassembler::inst_uses_and_defs_from_cfg(std::shared_ptr<CustomCFGImpl> cfg) {

    std::string first_ep = cfg->getEntry_point();
    std::string ret_str ="";
    std::unordered_set<std::string> inst_visited_map;
    std::unordered_set<std::string> block_visited_map;
    std::queue<std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>>> q;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> bbs = cfg->getBasic_blocks();
    q.push(std::make_pair(first_ep, bbs->at(first_ep)));
    while(!q.empty()){
        std::pair<std::string, std::shared_ptr<CustomBasicBlockImpl>> node = q.front();
        q.pop();
        if(block_visited_map.find(node.first) != block_visited_map.end()) continue;
        block_visited_map.insert(node.first);
        if(node.second->isIs_dead_end()) continue;
        std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> insts_of_this_b = node.second->getInstructions();
        for(std::vector<std::shared_ptr<X86Instruction>>::iterator ins_it = insts_of_this_b->begin();
            ins_it != insts_of_this_b->end(); ++ins_it){
            if(inst_visited_map.find((*ins_it)->getAddress()) != inst_visited_map.end()) continue;
            inst_visited_map.insert((*ins_it)->getAddress());
            ret_str += (*ins_it)->getAddress() + ":\n";
            ret_str += "\tUses:\n";
            for(std::unordered_set<std::string>::iterator uses_it = (*ins_it)->getRegs_read()->begin(); uses_it != (*ins_it)->getRegs_read()->end(); ++uses_it)
                ret_str += "\t\t" + *uses_it + "\n";

            ret_str += "\tDefs:\n";
            for(std::unordered_set<std::string>::iterator defs_it = (*ins_it)->getRegs_writen()->begin(); defs_it != (*ins_it)->getRegs_writen()->end(); ++defs_it)
                ret_str += "\t\t" + *defs_it + "\n";

        }

        if(node.second->isIs_dead_end()) continue;
        for(std::vector<std::shared_ptr<CustomBasicBlockImpl>>::iterator suc_it = node.second->getSucessors()->begin();
            suc_it!= node.second->getSucessors()->end(); ++suc_it)
            q.push(std::make_pair((*suc_it)->getEntry_point(), *suc_it));

    }
    return ret_str;

}

void Disassembler::write_asm_to_file(const std::string &file_addr, std::shared_ptr<DisassemblingInfo> inf) {

    std::ofstream o(file_addr);
    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> inss = inf->getIns_vector();

    for(std::vector<std::shared_ptr<X86Instruction>>::iterator it=inss->begin(); it!= inss->end(); ++it){

        std::shared_ptr<X86Instruction> i = (*it);
        o<< i->getAddress() + ": " + i->getMnemonic() + " " + i->getOp_str() << std::endl;

    }
    o.close();

}

std::pair<std::string, std::string> Disassembler::getEntryPointAndAssemblyFromCustomFile(std::string input_file) {

    std::ifstream _f(input_file);
    std::string to_return = "";
    std::string entry_point = "";
    bool got_entry_point = false;

    if(_f.is_open()){

        std::string line;
        while(getline(_f,line )){

                if(!got_entry_point){
                    entry_point = line;
                    got_entry_point = true;
                }
                else{
                    to_return += line;
                    }
                }

        _f.close();

    }

    else
        throw "Couldn't open file. " + input_file;

    return std::make_pair(entry_point, to_return);


}

std::shared_ptr<DisassemblingInfo>
Disassembler::dissasemble(std::string hex_dump, std::string md5, const std::string &hex_entry_point) {

    bool entry_point_saved = false;
    std::string entry_point = "";

    std::vector<char> bytes = Aux::HexToBytes(hex_dump);
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

    int ep = Aux::get_int_from_hex_string(hex_entry_point);

    count = cs_disasm(handle, CODE, bytes.size(), ep, 0, &insn);

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
    }
    else
        std::cerr<<"ERROR: Failed to disassemble given code!"<< std::endl;
    cs_close(&handle);

    std::shared_ptr<DisassemblingInfo> info = std::make_shared<DisassemblingInfo>(ins_addr_map, instructions,
                                                                                  entry_point, md5);
    return info;
}
