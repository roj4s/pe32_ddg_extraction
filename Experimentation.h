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
    static std::pair<std::shared_ptr<DisassemblingInfo>, double> disassembling_exp(std::string code_file_addr,
                                                                                   std::string md5,
                                                                                   const std::string &entry_point);

    static std::pair<std::shared_ptr<CustomCFGImpl>, double> get_cfg_exp(std::shared_ptr<DisassemblingInfo>);

    static double serialize_cfg_exp(std::shared_ptr<CustomCFGImpl>, std::string destination_address);

    static std::pair<std::shared_ptr<boost::DFGraph>, double> get_dfg_exp(std::shared_ptr<CustomCFGImpl>);

    static void batch_whole_arch_run(const std::string &db_addr, const std::string &output_folder,
                                     std::unordered_set<std::string> outliers);
    static double dfg_feature_extraction_for_etgm_delay(const std::string &md5,
                                                 const std::string &db_adddr,
                                                 std::shared_ptr<std::unordered_map<std::string,
                                                         std::string>> mnemonic_map,
                                                 std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>>
                                                        graphs);

    static double serialize_dfg_exp(boost::DFGraph graph, std::string output);
};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_EXPERIMENTATION_H
