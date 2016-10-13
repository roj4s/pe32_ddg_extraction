//
// Created by rojas on 11/10/16.
//

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H
#define ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H

#include "CustomBasicBlockImpl.h"

class CustomCFGImpl {

private:
    std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks;
    std::string entry_point;


public:
    CustomCFGImpl(const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &basic_blocks,
                  const std::string &entry_point);

    const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &getBasic_blocks() const;

    const std::string &getEntry_point() const;
};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H
