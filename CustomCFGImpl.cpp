//
// Created by rojas on 11/10/16.
//

#include "CustomCFGImpl.h"
#include "Disassembler.h"


CustomCFGImpl::CustomCFGImpl(
        const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &basic_blocks,
        const std::string &entry_point, const std::shared_ptr<std::unordered_set<std::string>> &visited_set,
        const std::shared_ptr<std::unordered_set<std::string>> &non_existent_addresses,
        const std::shared_ptr<std::unordered_set<std::string>> &call_return_addresses) : basic_blocks(basic_blocks),
                                                                                 entry_point(entry_point),
                                                                                 visited_set(visited_set),
                                                                                 non_existent_addresses(non_existent_addresses),
                                                                                 call_return_addresses(call_return_addresses) {}

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>>
CustomCFGImpl::getBasic_blocks() const {
    return basic_blocks;
}

const std::string &CustomCFGImpl::getEntry_point() const {
    return entry_point;
}

std::shared_ptr<std::unordered_set<std::string>> CustomCFGImpl::getVisited_set() const {
    return visited_set;
}

std::shared_ptr<std::unordered_set<std::string>> CustomCFGImpl::getNon_existent_addresses() const {
    return non_existent_addresses;
}

std::shared_ptr<std::unordered_set<std::string>> CustomCFGImpl::getCall_return_addresses() const {
    return call_return_addresses;
}

json CustomCFGImpl::get_bb_json(std::string from_this_address) {

    json json_of_this_bb;

    json_of_this_bb["entry_point"] = from_this_address;

    std::vector<std::string> instructions_vector;

    std::vector<json> next_bb_jsons;

    std::shared_ptr<CustomBasicBlockImpl> this_bb = this->basic_blocks->at(from_this_address);

    std::shared_ptr<std::vector<std::shared_ptr<X86Instruction>>> ins_of_this_bb = this_bb->getInstructions();

    std::shared_ptr<std::vector<std::shared_ptr<CustomBasicBlockImpl>>> next_bbs = this_bb->getSucessors();

    for(std::vector<std::shared_ptr<X86Instruction>>::iterator ins_it = ins_of_this_bb->begin();
        ins_it != ins_of_this_bb->end(); ++ins_it){

        instructions_vector.push_back((*ins_it)->getStr());

    }

    std::shared_ptr<std::unordered_map<std::string, std::string>> defs = this_bb->getDefs();
    json defs_json;

    for(std::unordered_map<std::string, std::string>::iterator def_it = defs->begin(); def_it != defs->end(); ++def_it)
        defs_json[def_it->first] = def_it->second;

    for(std::vector<std::shared_ptr<CustomBasicBlockImpl>>::iterator n_bb_it = next_bbs->begin(); n_bb_it != next_bbs->end(); ++n_bb_it){

        std::string entry_point_of_this_bb = (*n_bb_it)->getEntry_point();

        if(entry_point_of_this_bb != "terminal"){

            json next_bb_j = CustomCFGImpl::get_bb_json(entry_point_of_this_bb);

            next_bb_jsons.push_back(next_bb_j);
        }

    }

    json_of_this_bb["instructions"] = instructions_vector;
    json_of_this_bb["defs"] = defs_json;
    json_of_this_bb["next_bb"] = next_bb_jsons;

    return json_of_this_bb;
}

int CustomCFGImpl::serialize(std::string destination_address, int identation_size) {

    json cfg_json;

    cfg_json["entry_point"] = this->entry_point;

    cfg_json["visited_set"] = *(this->visited_set);

    cfg_json["non_existent_addresses"] = *(this->non_existent_addresses);

    cfg_json["call_return_addresses"] = *(this->call_return_addresses);

    cfg_json["basic_blocks"] = CustomCFGImpl::get_bb_json(this->entry_point);

    std::fstream _f;

    _f.open(destination_address, std::ios::out);

    _f << cfg_json.dump(identation_size) << std::endl;

    _f.close();


    return 0;
}
