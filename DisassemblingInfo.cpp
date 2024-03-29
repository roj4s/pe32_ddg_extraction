//
// Created by rojas on 11/10/16.
//

#include "DisassemblingInfo.h"


std::shared_ptr<std::unordered_map<std::string, int>> DisassemblingInfo::getIns_addr_map() const {
    return ins_addr_map;
}

std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> DisassemblingInfo::getIns_vector() const {
    return ins_vector;
}

DisassemblingInfo::DisassemblingInfo(const std::shared_ptr<std::unordered_map<std::string, int>> &ins_addr_map,
                                     const std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> &ins_vector,
                                     const std::string &entry_point, std::string md5) : ins_addr_map(ins_addr_map),
                                                                       ins_vector(ins_vector),
                                                                       entry_point(entry_point), md5(md5) {}

const std::string &DisassemblingInfo::getEntry_point() const {
    return entry_point;
}

const std::string &DisassemblingInfo::getMd5() const {
    return md5;
}
