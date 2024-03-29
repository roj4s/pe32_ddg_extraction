//
// Created by rojas on 20/10/16.
//

#include "MnemonicClassification.h"
#include "Aux.h"


std::shared_ptr<std::unordered_map<std::string, std::string>>
MnemonicClassification::get_from_json_file(std::string file_addr) {

    std::shared_ptr<std::unordered_map<std::string, std::string>> to_ret = std::make_shared<std::unordered_map<std::string, std::string>>();

    std::string json_str = Aux::get_string_from_file(file_addr);
    json _j = json::parse(json_str);
    std::unordered_set<std::string> k = _j["keys"];

    for(std::string m :k){
        std::string val = _j[m];
        to_ret->insert({m, val});
    }

    return to_ret;
}
