//
// Created by rojas on 11/10/16.
//

#include <iostream>
#include <unordered_map>
#include <memory>
#include <capstone/capstone.h>
#include <vector>
#include "X86Instruction.h"

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLINGINFO_H
#define ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLINGINFO_H



class DisassemblingInfo {
public:

    DisassemblingInfo(const std::shared_ptr<std::unordered_map<std::string, int>> &ins_addr_map,
                      const std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> &ins_vector,
                      const std::string &entry_point, std::string md5);

private:
    std::shared_ptr<std::unordered_map<std::string,int>> ins_addr_map;
    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> ins_vector;
    std::string entry_point;
    std::string md5;

public:
    const std::string &getMd5() const;
    std::shared_ptr<std::unordered_map<std::string, int>> getIns_addr_map() const;
    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> getIns_vector() const;

    const std::string &getEntry_point() const;
};




#endif //ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLINGINFO_H
