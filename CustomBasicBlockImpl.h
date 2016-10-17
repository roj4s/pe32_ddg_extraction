//
// Created by rojas on 11/10/16.
//

#include <memory>
#include <capstone/capstone.h>
#include <vector>
#include <unordered_map>

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMBASICBLOCKIMPL_H
#define ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMBASICBLOCKIMPL_H





class CustomBasicBlockImpl {

private:
    std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> instructions;
    std::shared_ptr<std::unordered_map<std::string, int>> ins_map;
    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> next_blocks;
    std::string entry_point;
    bool is_dead_end;
    bool root_bb;

public:
    CustomBasicBlockImpl(const std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> &instructions,
                         const std::shared_ptr<std::unordered_map<std::string, int>> &ins_map,
                         const std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> &next_blocks,
                         const std::string &entry_point);

    CustomBasicBlockImpl(bool);

    bool isIs_dead_end() const;

    void add_next_block(std::shared_ptr<CustomBasicBlockImpl> bb);

    std::shared_ptr<std::vector<std::shared_ptr<cs_insn>>> getInstructions() const;

    std::shared_ptr<std::unordered_map<std::string, int>> getIns_map() const;

    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> getNext_blocks() const;

    const std::string &getEntry_point() const;

    bool isRoot_bb() const;
};




#endif //ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMBASICBLOCKIMPL_H
