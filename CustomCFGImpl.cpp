//
// Created by rojas on 11/10/16.
//

#include "CustomCFGImpl.h"

CustomCFGImpl::CustomCFGImpl(const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &basic_blocks,
                             const std::string &entry_point) : basic_blocks(basic_blocks), entry_point(entry_point) {}

const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &CustomCFGImpl::getBasic_blocks() const {
    return basic_blocks;
}

const std::string &CustomCFGImpl::getEntry_point() const {
    return entry_point;
}
