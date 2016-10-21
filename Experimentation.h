//
// Created by rojas on 13/10/16.
//

#include "DisassemblingInfo.h"
#include "CustomCFGImpl.h"
#include "DFG.h"

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_EXPERIMENTATION_H
#define ARCH_UNTIL_FEATURESEXTRACTION_EXPERIMENTATION_H




class Experimentation {

public:
    static std::pair<std::shared_ptr<DisassemblingInfo>, double> disassembling_exp(std::string code_file_addr, std::string md5);

    static std::pair<std::shared_ptr<CustomCFGImpl>, double> get_cfg_exp(std::shared_ptr<DisassemblingInfo>);

    static double serialize_cfg_exp(std::shared_ptr<CustomCFGImpl>, std::string destination_address);
    static std::pair<std::shared_ptr<boost::DFGraph>, double> get_dfg_exp(std::shared_ptr<CustomCFGImpl>);

};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_EXPERIMENTATION_H
