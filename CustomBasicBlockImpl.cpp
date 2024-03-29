//
// Created by rojas on 11/10/16.
//

#include "CustomBasicBlockImpl.h"
#include "Disassembler.h"


std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> CustomBasicBlockImpl::getInstructions() const {
    return instructions;
}

std::shared_ptr<std::unordered_map<std::string, int>> CustomBasicBlockImpl::getIns_map() const {
    return ins_map;
}

std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>>
CustomBasicBlockImpl::getSucessors() const {
    return sucessors;
}



bool CustomBasicBlockImpl::isIs_dead_end() const {
    return is_dead_end;
}


CustomBasicBlockImpl::CustomBasicBlockImpl(bool is_dead_end) {

    this->is_dead_end = is_dead_end;
    this->root_bb = !is_dead_end;

    if(!is_dead_end)
        this->sucessors = std::make_shared<std::vector<std::shared_ptr<CustomBasicBlockImpl>>>();

    entry_point = "terminal";

}

bool CustomBasicBlockImpl::isRoot_bb() const {
    return root_bb;
}

CustomBasicBlockImpl::CustomBasicBlockImpl(const std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> &instructions,
                                           const std::shared_ptr<std::unordered_map<std::string, int>> &ins_map,
                                           const std::string &entry_point) : instructions(instructions),
                                                                                           ins_map(ins_map),
                                                                                           entry_point(entry_point) {
    this->is_dead_end = false;
    this->root_bb = false;

    this->sucessors = std::make_shared<std::vector<std::shared_ptr<CustomBasicBlockImpl>>>();
    this->predecessors = std::make_shared<std::vector<std::shared_ptr<CustomBasicBlockImpl>>>();

    this->sucessors_addresses = std::make_shared<std::unordered_set<std::string>>();
    this->predecessors_addresses = std::make_shared<std::unordered_set<std::string>>();
//
//
//    for(std::vector<std::shared_ptr<X86Instruction>>::iterator ins_it = instructions->begin(); ins_it != instructions->end();
//        ++ins_it){
//
//        std::shared_ptr<X86Instruction> ins = (*ins_it);
//        std::string addr_str = ins->getAddress();
//        unsigned int addr_int = std::stoul(addr_str, nullptr, 16);
//
//        std::shared_ptr<std::unordered_set<std::string>> regs_writen = ins->getRegs_writen();
//
//        for(std::unordered_set<std::string>::iterator regs_w_it = regs_writen->begin(); regs_w_it != regs_writen->end(); ++regs_w_it){
//
//            std::string reg = (*regs_w_it);
//            std::unordered_map<std::string, std::shared_ptr<std::vector<std::string>>>::iterator find_addr = this->defs->find(reg);
//
//            if(find_addr == this->defs->end() || addr_int > std::stoul(find_addr->second->at(0), nullptr, 16)){
//                std::shared_ptr<std::vector<std::string>> n_v = std::make_shared<std::vector<std::string>>();
//                n_v->push_back(addr_str);
//                this->defs->insert({reg, n_v});
//            }
//
//        }
//    }
}

const std::string &CustomBasicBlockImpl::getEntry_point() const {
    return entry_point;
}

void CustomBasicBlockImpl::add_predecessor(std::shared_ptr<CustomBasicBlockImpl> bb) {

    if(this->entry_point != "terminal"){
        if(this->predecessors_addresses->find(bb->getEntry_point()) == this->predecessors_addresses->end()){
            this->predecessors->push_back(bb);
            this->predecessors_addresses->insert(bb->getEntry_point());
        }
    }

}

void CustomBasicBlockImpl::add_sucessor(std::shared_ptr<CustomBasicBlockImpl> bb) {

    if(this->sucessors_addresses->find(bb->getEntry_point()) == this->sucessors_addresses->end()){
        this->sucessors->push_back(bb);
        this->sucessors_addresses->insert(bb->getEntry_point());
    }


}

std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> CustomBasicBlockImpl::getPredecessors() const {
    return this->predecessors;
}

std::shared_ptr<std::unordered_set<std::string>> CustomBasicBlockImpl::getSucessors_addresses() const {
    return sucessors_addresses;
}

std::shared_ptr<std::unordered_set<std::string>> CustomBasicBlockImpl::getPredecessors_addresses() const {
    return predecessors_addresses;
}
//
//std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::vector<std::string>>>> CustomBasicBlockImpl::getDefs() const {
//    return defs;
//}
