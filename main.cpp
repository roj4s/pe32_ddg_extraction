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
#include <dirent.h>
#include "MalwareOrganizedTools.h"

int main_1(void) {

    std::string exec_info_db_addr = "/home/rojas/mestrado/bening_instances/code_and_entry_point/execinfo.db";
    std::string output_folder = "/home/rojas/mestrado/bening_instances/fvs/";
    std::unordered_set<std::string> outliers = {};
    outliers.insert("c93cb963a081533994d59bf806aec1ff");

    std::unordered_set<std::string> dbs ={};
//    std::unordered_set<std::string> dbs ={"/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:09:22/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:20:34/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:19:49/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:18:00/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:18:49/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:17:26/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Thu_Nov_2016-11-03_17:06:04/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:16:15/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:11:28/output_db.db","/home/rojas/dev/msc_thesis_dev/outputs/fvs_for_training/whole_arch_exp__Fri_Nov_2016-11-04_14:11:58/output_db.db"};
    std::string base_db_dir = "/home/rojas/mestrado/bening_instances/fvs/";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (base_db_dir.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {

            if(strcmp(ent->d_name,".") !=0 && strcmp(ent->d_name,"..") != 0) {
                dbs.insert(base_db_dir + ent->d_name + "/output_db.db");
//                printf ("%s\n", base_db_dir + std::string(ent->d_name) + "/output_db.db");
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("Couldnt open dir");
        return EXIT_FAILURE;
    }

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


int main(void){

    std::string mestrado_root = "/home/rojas/mestrado/";
    std::string malware_organized_root = mestrado_root + "malware_organized/";
    std::string seeds_root = mestrado_root + "malware_organized/seeds/";
    std::string r4_generated_root = mestrado_root + "malware_organized/r4_generated/";
    std::string exec_code_and_entry_point_files_root = seeds_root + "exec_code_and_entry_point/";
    std::string asm_code_files_root = seeds_root + "assembly_code/";

    MalwareOrganizedTools mot(malware_organized_root);
    mot.update_seeds_cfg();


}