//
// Created by rojas on 11/10/16.
//



#ifndef ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLINGINFO_H
#define ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLINGINFO_H

#include <iostream>
#include <unordered_map>
#include <memory>
#include <capstone/capstone.h>
#include <vector>

class DisassemblingInfo {
public:

    DisassemblingInfo(const std::shared_ptr<std::unordered_map<std::string, int>> &ins_addr_map,
                      const std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> &ins_vector, const std::string &entry_point);

private:
    std::shared_ptr<std::unordered_map<std::string,int>> ins_addr_map;
    std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> ins_vector;
    std::string entry_point;

public:
    std::shared_ptr<std::unordered_map<std::string, int>> getIns_addr_map() const;
    std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> getIns_vector() const;

    const std::string &getEntry_point() const;
};




#endif //ARCH_UNTIL_FEATURESEXTRACTION_DISASSEMBLINGINFO_H
