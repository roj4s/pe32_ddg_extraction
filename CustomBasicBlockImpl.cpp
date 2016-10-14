//
// Created by rojas on 11/10/16.
//

#include "CustomBasicBlockImpl.h"


std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> CustomBasicBlockImpl::getInstructions() const {
    return instructions;
}

std::shared_ptr<std::unordered_map<std::string, int>> CustomBasicBlockImpl::getIns_map() const {
    return ins_map;
}

std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>>
CustomBasicBlockImpl::getNext_blocks() const {
    return next_blocks;
}

void CustomBasicBlockImpl::add_next_block(std::shared_ptr<CustomBasicBlockImpl> bb) {

    this->next_blocks->push_back(bb);

}

bool CustomBasicBlockImpl::isIs_dead_end() const {
    return is_dead_end;
}


CustomBasicBlockImpl::CustomBasicBlockImpl(bool is_dead_end) {

    this->is_dead_end = is_dead_end;
    this->root_bb = !is_dead_end;

    if(!is_dead_end)
        this->next_blocks = std::make_shared<std::vector<std::shared_ptr<CustomBasicBlockImpl>>>();

    entry_point = "terminal";

}

bool CustomBasicBlockImpl::isRoot_bb() const {
    return root_bb;
}

CustomBasicBlockImpl::CustomBasicBlockImpl(const std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> &instructions,
                                           const std::shared_ptr<std::unordered_map<std::string, int>> &ins_map,
                                           const std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> &next_blocks,
                                           const std::string &entry_point, bool root_bb) : instructions(instructions),
                                                                                           ins_map(ins_map),
                                                                                           next_blocks(next_blocks),
                                                                                           entry_point(entry_point),
                                                                                           root_bb(root_bb) {
    this->is_dead_end = false;

}

const std::string &CustomBasicBlockImpl::getEntry_point() const {
    return entry_point;
}
