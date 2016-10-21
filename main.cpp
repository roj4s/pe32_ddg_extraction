#include "Experimentation.h"
#include "Disassembler.h"
#include "DFG.h"
#include "MnemonicClassification.h"
#include "FeaturesExtraction.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>

int main(void) {

    std::string addr_of_code_file = "/home/rojas/dev/msc_thesis_dev/arch_c/code_sample";
    std::string serialized_cfg_addr = "/tmp/serialized_cfg.json";
//
    std::pair<std::shared_ptr<DisassemblingInfo>, double> disassembling_exp_results = Experimentation::disassembling_exp(addr_of_code_file, "abcdef");
//
    std::pair<std::shared_ptr<CustomCFGImpl>, double> cfg_exp_results = Experimentation::get_cfg_exp(disassembling_exp_results.first);
//
//    std::cout<< cfg_exp_results.first->getEntry_point() << std::endl;
//
//    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> bbs = cfg_exp_results.first->getBasic_blocks();
//
//    for(std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>::iterator it = bbs->begin(); it != bbs->end(); ++it)
//        std::cout << it->first <<std::endl;
//
//    double delay_of_cfg_serializing = Experimentation::serialize_cfg_exp(cfg_exp_results.first, serialized_cfg_addr);
//
    std::pair<std::shared_ptr<boost::DFGraph>, double> dfg_inf = Experimentation::get_dfg_exp(cfg_exp_results.first);


//    std::cout << "Delay on constructing DFG: " << std::to_string(dfg_inf.second) << std::endl;
//
    std::shared_ptr<boost::DFGraph> g = dfg_inf.first;
    boost::DFGMd5 m = boost::get(boost::md5_t(), *g);
//    std::string h = boost::get(m, *g)
    std::cout << m << std::endl;
//    boost::DFGVInststrMap inststr = boost::get(boost::ins_str_t(), *g);
//    boost::DFGVRegUsedMap regUsedMap = boost::get(boost::reg_used_t(), *g);
//    boost::DFGVAddressMap addressMap = boost::get(boost::address_hex_str_t(), *g);
//
//
//    std::string a = Disassembler::inst_uses_and_defs_from_cfg(cfg_exp_results.first);
//    std::cout << a << std::endl;
//    boost::DFGraph h = *g;
//    boost::write_graphviz(std::cout, h, boost::make_label_writer(addressMap), boost::make_label_writer(regUsedMap));

//    std::string mnemonic_class_file = "/home/rojas/dev/msc_thesis_dev/arch_c/mnemonic_classifications/geek32_abc";
//    std::shared_ptr<std::unordered_map<std::string, std::string>> mnemonic_map = MnemonicClassification::get_from_json_file(mnemonic_class_file);
//    std::string db_a = "/tmp/db.db";
//    std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>> graphs = std::make_shared<std::unordered_set<std::shared_ptr<boost::DFGraph>>>();
//    graphs->insert(dfg_inf.first);
//
//    FeaturesExtraction::extraction_for_etgm(graphs, mnemonic_map, db_a);

}