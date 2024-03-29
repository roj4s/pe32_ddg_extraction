//
// Created by rojas on 20/10/16.
//

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_MNEMONICCLASSIFICATION_H
#define ARCH_UNTIL_FEATURESEXTRACTION_MNEMONICCLASSIFICATION_H


#include <unordered_map>
#include <memory>
#include "deps/json.hpp"

using json = nlohmann::json ;

class MnemonicClassification {

public:
    static std::shared_ptr<std::unordered_map<std::string, std::string>> get_from_json_file(std::string file_addr);

};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_MNEMONICCLASSIFICATION_H
