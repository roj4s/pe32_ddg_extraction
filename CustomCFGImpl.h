//
// Created by rojas on 11/10/16.
//



#ifndef ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H
#define ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H


#include <unordered_set>
#include <queue>
#include "CustomBasicBlockImpl.h"
#include "deps/json.hpp"
#include <fstream>

using json = nlohmann::json ;

class CustomCFGImpl {

private:
    std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks;
    std::string entry_point;
    std::shared_ptr<std::unordered_set<std::string>> visited_set;
    std::shared_ptr<std::unordered_set<std::string>> non_existent_addresses;
    std::shared_ptr<std::unordered_set<std::string>> call_return_addresses;
    std::string md5;
public:
    const std::string &getMd5() const;

public:
    CustomCFGImpl(
            const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &basic_blocks,
            const std::string &entry_point, const std::shared_ptr<std::unordered_set<std::string>> &visited_set,
            const std::shared_ptr<std::unordered_set<std::string>> &non_existent_addresses,
            const std::shared_ptr<std::unordered_set<std::string>> &call_return_addresses, std::string md5);

    int serialize(std::string destination_address, int identation_size =4);

    json get_bb_json(std::string from_this_address);

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>>
    getBasic_blocks() const;

    const std::string &getEntry_point() const;

    std::shared_ptr<std::unordered_set<std::string>> getVisited_set() const;

    std::shared_ptr<std::unordered_set<std::string>> getNon_existent_addresses() const;

    std::shared_ptr<std::unordered_set<std::string>> getCall_return_addresses() const;
};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H
