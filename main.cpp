#include "Experimentation.h"
#include "Disassembler.h"
#include "DFG.h"
#include "MnemonicClassification.h"
#include "FeaturesExtraction.h"
#include "Aux.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>

int main(void) {

    std::string exec_info_db_addr = "/home/rojas/dev/msc_thesis_dev/outputs/code_and_entry_point_testing/execinfo.db";
    std::string output_folder = "/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_testing/";
    std::unordered_set<std::string> outliers = {};
    outliers.insert("2879e58e118ff0a8669e0b920886f2c7");
    outliers.insert("a3e360e5ea40242d3fd4124239d0ea8e");
    outliers.insert("662f9158a5ffcc735f603d136ce742ec");
    outliers.insert("3847944a569c681d845b39dd7e666325");
    outliers.insert("dc7b53038c3dfeb1eb4f464d7a30ff53");
    outliers.insert("fab7696390f4456f2658703a1c8dad4f");
    outliers.insert("0a1aa3e213c9a9042abbebd40d85da2c");
    outliers.insert("810604cd99d590bd1da28905b5a5f976");
    outliers.insert("c7377a84c03776029889774e7d1ba097");
    outliers.insert("0ada1119c5dfbbc45cd76cf3a0e1bd43");
    outliers.insert("375576bad37983cd1240c86f518c674d");
    outliers.insert("59db805b0125709a365e84d8733bb753");
    outliers.insert("b9b7ea3defd41af00931ffc2ba2615d8");
    outliers.insert("20327d6ea0476d644c383fed1e3effca");
    outliers.insert("39db7e38db28c0af7d5f300395d09be0");
    outliers.insert("a86021a45c3c00b85285c9875478031d");
    outliers.insert("23e70dc891fe7ef7b478dd38004f0128");
    outliers.insert("03c93aa34a7d7ce535ca5bc25825807c");
    outliers.insert("93c2c62c485b298fa602af3407e573e9");
    outliers.insert("0368b6cfaacefea2a1febdd3347e4b4c");
    outliers.insert("668f30a0cdb6d6881658f7697d336491");
    outliers.insert("a3027dbadeed5dbd0abe61ab291b2105");
    outliers.insert("e7bd868fcbf16e13756f547f016a62fc");
    outliers.insert("5597ab258cb5d4a29a3be1de5ee75db4");
    outliers.insert("516189198da6418f52b8cc1a2558497c");
    outliers.insert("660eb665063d2b2a9ee288e9d8bd394e");
    outliers.insert("8582d4e4ad962d378e4d4a203ac54ade");
    outliers.insert("751998344e7d02603b2ca90fd10182be");
    outliers.insert("f4914a2837a01d81b5cd0a8c339b98de");
    outliers.insert("6b3673ffc12c9dc0baf4148fc73499ff");
    outliers.insert("377f686650fed5442304af7d4db03d7f");
    outliers.insert("0d57f0b4f0281a32334d565218c87326");
    outliers.insert("adeceaa2055e38d121be056280e4dfc1");
    outliers.insert("85a65cd0146355f1e3e42755e4feaeed");
    outliers.insert("26fe1b243deeabb7ac3d54494ccacb17");
    outliers.insert("a031c7f85d1f4a34f9750648e0d5e613");
    outliers.insert("27d338b9b93ba2cf1192ba6148fb6475");
    outliers.insert("e4a3bdb917f10bdd682dd113c80fdee4");
    outliers.insert("a9248d791a1632c0e5c36b8bb4a54718");
    outliers.insert("6821c9c23713281e63a68042a48df173");
    outliers.insert("af72e7a78670db2b8d26e452eec8fd4c");
    outliers.insert("7691269370b88cdc4e0a7c6fb7ad1780");
    outliers.insert("b5651caddbc0b3ec42b19da5e0b27278");
    outliers.insert("634be17e5570c260b49b2639c35d40ff");

    std::unordered_set<std::string> dbs ={};
//    std::unordered_set<std::string> dbs ={"/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:09:22/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:20:34/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:19:49/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:18:00/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:18:49/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:17:26/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Thu_Nov_2016-11-03_17:06:04/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:16:15/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:11:28/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:11:58/output_db.db"};

    for(std::string db_addr: dbs){


        sqlite3 * db = Aux::get_db_connection(db_addr);
        std::string query = "select md5 from batch_whole_arch_run;";
        sqlite3_stmt * stmnt;

        if(sqlite3_prepare(db, query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
        {

            int res = 0;
            while(1){
                res = sqlite3_step(stmnt);
                if(res == SQLITE_ROW)
                    outliers.insert(reinterpret_cast<const char*>(sqlite3_column_text(stmnt,0)));
                if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
            }
        }
    }

    std::cout<< outliers.size() << std::endl;

//
    Experimentation::batch_whole_arch_run(exec_info_db_addr, output_folder, outliers);


//
////
//    std::string addr_of_code_file = "/home/rojas/dev/msc_thesis_dev/arch_c/code_sample";
//    std::string addr_of_code_file = "/home/rojas/dev/Python/Malware_Indexing/experiments_outputs/malware_instances/2879e58e118ff0a8669e0b920886f2c7";
//    std::string serialized_cfg_addr = "/tmp/serialized_cfg.json";
//////
//    std::pair<std::shared_ptr<DisassemblingInfo>, double> disassembling_exp_results = Experimentation::disassembling_exp(addr_of_code_file, "abcdef", "0x401000");
//    int a = disassembling_exp_results.first->getIns_vector()->size();

//////
//    Disassembler::write_asm_to_file("/tmp/asm.tmp",disassembling_exp_results.first);
//    std::pair<std::shared_ptr<CustomCFGImpl>, double> cfg_exp_results = Experimentation::get_cfg_exp(disassembling_exp_results.first);
//    a = cfg_exp_results.first->getBasic_blocks()->size();


//    std::shared_ptr<boost::CFG> cfg = cfg_exp_results.first->get_cfg_as_boost();
//    boost::CFGInstructionsMap imap = boost::get(boost::instructions(), *cfg);

//    std::ofstream of("/tmp/cfg.dot");
//    boost::write_graphviz(of, *cfg, boost::make_label_writer(imap));

////
////    std::cout<< cfg_exp_results.first->getEntry_point() << std::endl;
////ins
////    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> bbs = cfg_exp_results.first->getBasic_blocks();
////
////    for(std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>::iterator it = bbs->begin(); it != bbs->end(); ++it)
////        std::cout << it->first <<std::endl;
////
////    double delay_of_cfg_serializing = Experimentation::serialize_cfg_exp(cfg_exp_results.first, serialized_cfg_addr);
////
//    std::pair<std::shared_ptr<boost::DFGraph>, double> dfg_inf = Experimentation::get_dfg_exp(cfg_exp_results.first);
//
////    double ser_dfg_time = Experimentation::serialize_dfg_exp(*(dfg_inf.first), "/tmp/dfgserilized.dot");
//
////    std::cout << "Delay on constructing DFG: " << std::to_string(dfg_inf.second) << std::endl;
////
////    std::shared_ptr<boost::DFGraph> g = dfg_inf.first;
////    boost::DFGMd5 m = boost::get(boost::md5_t(), *g);
////    std::string h = boost::get(m, *g)
////    std::cout << m << std::endl;
////    boost::DFGVInststrMap inststr = boost::get(boost::ins_str_t(), *g);
////    boost::DFGVRegUsedMap regUsedMap = boost::get(boost::reg_used_t(), *g);
////    boost::DFGVAddressMap addressMap = boost::get(boost::address_hex_str_t(), *g);
////
////
////    std::string a = Disassembler::inst_uses_and_defs_from_cfg(cfg_exp_results.first);
////    std::cout << a << std::endl;
////    boost::DFGraph h = *g;
////    boost::write_graphviz(std::cout, h, boost::make_label_writer(addressMap), boost::make_label_writer(regUsedMap));
//
//    std::string mnemonic_class_file = "/home/rojas/dev/msc_thesis_dev/arch_c/mnemonic_classifications/geek32_abc";
//    std::shared_ptr<std::unordered_map<std::string, std::string>> mnemonic_map = MnemonicClassification::get_from_json_file(mnemonic_class_file);
//    std::string db_a = "/tmp/db.db";
//    std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>> graphs = std::make_shared<std::unordered_set<std::shared_ptr<boost::DFGraph>>>();
//    graphs->insert(dfg_inf.first);
////
//    FeaturesExtraction::extraction_for_etgm(graphs, mnemonic_map, db_a, "45634");

}