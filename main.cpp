#include "Experimentation.h"
#include "Disassembler.h"
#include "DFG.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>


int main(void) {


//    std::string addr_of_code_file = "/home/rojas/Dev/Python/Malware_Indexing/arch_c/code_sample";
//    std::string addr_of_code_file = "/home/rojas/dev/msc_thesis_dev/arch_c/code_sample";
//    std::string serialized_cfg_addr = "/tmp/serialized_cfg.json";
//
//    std::pair<std::shared_ptr<DisassemblingInfo>, double> disassembling_exp_results = Experimentation::disassembling_exp(addr_of_code_file);
//
//    std::pair<std::shared_ptr<CustomCFGImpl>, double> cfg_exp_results = Experimentation::get_cfg_exp(disassembling_exp_results.first);
//
//    std::cout<< cfg_exp_results.first->getEntry_point() << std::endl;
//
//    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> bbs = cfg_exp_results.first->getBasic_blocks();
//
//    for(std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>::iterator it = bbs->begin(); it != bbs->end(); ++it)
//        std::cout << it->first <<std::endl;
////
//
////    std::cout << Disassembler::hex_from_int64(1000) << std::endl;
//
//    double delay_of_cfg_serializing = Experimentation::serialize_cfg_exp(cfg_exp_results.first, serialized_cfg_addr);


}