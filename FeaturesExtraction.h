//
// Created by rojas on 20/10/16.
//

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_FEATURESEXTRACTION_H
#define ARCH_UNTIL_FEATURESEXTRACTION_FEATURESEXTRACTION_H


#include <sqlite3.h>
#include "DFG.h"
#include "deps/json.hpp"

using json = nlohmann::json ;

class FeaturesExtraction {

    public:
        static void extraction_for_etgm(std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>> graphs,
                                        std::shared_ptr<std::unordered_map<std::string, std::string>> mnemonic_classification,
                                        std::string dest_db_address, std::string md5);

};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_FEATURESEXTRACTION_H
