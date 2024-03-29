//
// Created by rojas on 11/10/16.
//

#include <memory>
#include <capstone/capstone.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "X86Instruction.h"

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMBASICBLOCKIMPL_H
#define ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMBASICBLOCKIMPL_H





class CustomBasicBlockImpl {

private:
    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> instructions;
    std::shared_ptr<std::unordered_map<std::string, int>> ins_map;
    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> sucessors;
    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> predecessors;

    std::shared_ptr<std::unordered_set<std::string>> sucessors_addresses;
public:
    std::shared_ptr<std::unordered_set<std::string>> getSucessors_addresses() const;

    std::shared_ptr<std::unordered_set<std::string>> getPredecessors_addresses() const;

private:
    std::shared_ptr<std::unordered_set<std::string>> predecessors_addresses;

    std::string entry_point;
    bool is_dead_end;
    bool root_bb;

public:
    CustomBasicBlockImpl(const std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> &instructions,
                         const std::shared_ptr<std::unordered_map<std::string, int>> &ins_map,
                         const std::string &entry_point);

//    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::vector<std::string>>>> getDefs() const;

    CustomBasicBlockImpl(bool);

    bool isIs_dead_end() const;

    void add_sucessor(std::shared_ptr<CustomBasicBlockImpl> bb);
    void add_predecessor(std::shared_ptr<CustomBasicBlockImpl> bb);

    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> getInstructions() const;

    std::shared_ptr<std::unordered_map<std::string, int>> getIns_map() const;

    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> getSucessors() const;
    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> getPredecessors() const;

    const std::string &getEntry_point() const;

    bool isRoot_bb() const;
};




#endif //ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMBASICBLOCKIMPL_H
