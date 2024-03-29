//
// Created by rojas on 13/10/16.
//

#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphviz.hpp>
#include <ios>
#include "Experimentation.h"
#include "Disassembler.h"
#include "Aux.h"
#include "MnemonicClassification.h"
#include "FeaturesExtraction.h"

std::pair<std::shared_ptr<DisassemblingInfo>, double> Experimentation::disassembling_exp(std::string code_file_addr,
                                                                                         std::string md5,
                                                                                         const std::string &entry_point) {

    clock_t delay_i = clock();
    std::shared_ptr<DisassemblingInfo> inf = Disassembler::dissasemble_from_hexdump_in_file(code_file_addr, md5,
                                                                                            entry_point);
    clock_t delay_e = clock();
    double delay = double(delay_e - delay_i)/CLOCKS_PER_SEC;
    return std::make_pair(inf, delay);


}

std::pair<std::shared_ptr<CustomCFGImpl>, double> Experimentation::get_cfg_exp(std::shared_ptr<DisassemblingInfo> d_inf) {

    clock_t delay_i = clock();
    std::shared_ptr<CustomCFGImpl> inf = Disassembler::get_cfg(d_inf);
    clock_t delay_e = clock();
    double delay = double(delay_e - delay_i)/CLOCKS_PER_SEC;
    return std::make_pair(inf, delay);

}

double Experimentation::serialize_cfg_exp(std::shared_ptr<CustomCFGImpl> cfg, std::string destination_address) {

    clock_t delay_i = clock();

    cfg->serialize(destination_address);

    clock_t delay_e = clock();
    double delay = double(delay_e - delay_i)/CLOCKS_PER_SEC;
    return delay;
}

std::pair<std::shared_ptr<boost::DFGraph>, double> Experimentation::get_dfg_exp(std::shared_ptr<CustomCFGImpl> cfg) {

    clock_t delay_i = clock();
    std::shared_ptr<boost::DFGraph> inf = Disassembler::get_dfg(cfg);

    clock_t delay_e = clock();
    double delay = double(delay_e - delay_i)/CLOCKS_PER_SEC;
    return std::make_pair(inf, delay);

}

void Experimentation::batch_whole_arch_run(const std::string &db_addr, const std::string &output_folder,
                                           std::unordered_set<std::string> outliers) {

    bool err = false;
    std::string errs = "";

    if(!Aux::file_exists(output_folder))
    {
        err= true;
        errs += "Specified output folder dont exists\n";
    }
    if(!Aux::file_exists(db_addr))
    {
        err= true;
        errs += "Specified db addr dont exists\n";
    }

    std::string select_query = "SELECT md5, file_location, entry_point, code_file_location FROM  exec_info;";
    std::string root_folder = output_folder + "/" + "whole_arch_exp_" + Aux::get_time_sufix() ;
    Aux::mkdir(root_folder);
    std::string error_log_file_addr = root_folder + "/errors.log";
    std::ofstream error_log_fstream(error_log_file_addr);

    std::string output_db = root_folder + "/output_db.db";
    std::string features_db = root_folder + "/fvs.db";
    std::string serialized_graphs_folder = root_folder + "/serialized_graphs/";
    Aux::mkdir(serialized_graphs_folder);
    std::string assembly_codes_folder = root_folder + "/asm/";
    Aux::mkdir(assembly_codes_folder);
    std::string cfgs_folder = root_folder + "/cfg/";
    Aux::mkdir(cfgs_folder);
    std::string dfgs_folder = root_folder + "/dfg/";
    Aux::mkdir(dfgs_folder);

    std::string mnemonic_class_file = "/home/rojas/mestrado/arch_c/mnemonic_classifications/geek32_abc";
    std::shared_ptr<std::unordered_map<std::string, std::string>> mnemonic_map = MnemonicClassification::get_from_json_file(mnemonic_class_file);

    std::string data_db_create_query = "CREATE TABLE batch_whole_arch_run(md5 varchar, disassembling_delay float, "
            "cfg_construction_delay float, cfg_serialization_delay float, dfg_construction_delay float,"
            " dfg_serialization_delay float, feature_extraction_delay float, type_of_features varchar, "
            "type_of_extraction varchar, quantity_of_instructions integer, quantity_of_basic_blocks integer,"
            " quantity_of_nodes_dfg integer);";

    sqlite3 * o_db = Aux::create_db(output_db, data_db_create_query);

    std::string data_db_insert_query_START = "INSERT INTO batch_whole_arch_run(md5, disassembling_delay,"
            " cfg_construction_delay, cfg_serialization_delay, dfg_construction_delay,"
            " dfg_serialization_delay, feature_extraction_delay, type_of_features, type_of_extraction,"
            " quantity_of_instructions, quantity_of_basic_blocks, quantity_of_nodes_dfg) VALUES (";

    sqlite3 * i_db = Aux::get_db_connection(db_addr);
    sqlite3_stmt *stmnt;

    int inserted_tuples = 0;


    if(sqlite3_prepare(i_db, select_query.c_str(), -1, &stmnt, 0) == SQLITE_OK){int res = 0;while(1){
            std::string iii = std::to_string(inserted_tuples);
            std::cout << "Inserted: " << iii << std::endl;
            res = sqlite3_step(stmnt); if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;if(res == SQLITE_ROW){

                try{

                    std::string md5  = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmnt, 0)));

                    std::cout << "Analysing: " << md5 << ":"<< std::endl;
//                    if(md5!="2879e58e118ff0a8669e0b920886f2c7") continue;
                    if(outliers.find(md5) != outliers.end())
                    {
                        std::cout << "\t Element is outlier" << std::endl;
                        inserted_tuples ++;
                        continue;
                    }


                    std::string file_location  = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmnt, 1)));
                    std::string entry_point  = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmnt, 2)));
                    std::string code_file_location  = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmnt, 3)));

                    std::string asm_file_addr = assembly_codes_folder + md5 + ".asm";
                    std::pair<std::shared_ptr<DisassemblingInfo>, double> disassembling_exp_results = Experimentation::disassembling_exp(code_file_location, md5, entry_point);
                    double disassembling_delay = disassembling_exp_results.second;
                    std::cout << "\tDisassembling delay: " << disassembling_delay << std::endl;
                    int quantity_of_instructions = disassembling_exp_results.first->getIns_vector()->size();
                    std::cout << "\tQuanity of instructions: " << quantity_of_instructions << std::endl;
                    Disassembler::write_asm_to_file(asm_file_addr, disassembling_exp_results.first);
                    std::pair<std::shared_ptr<CustomCFGImpl>, double> cfg_exp_results = Experimentation::get_cfg_exp(disassembling_exp_results.first);
                    double cfg_construction_delay = cfg_exp_results.second;
                    std::cout << "\tCfg Construction delay: " << cfg_construction_delay << std::endl;
                    int quantity_of_basic_blocks = cfg_exp_results.first->getBasic_blocks()->size();
                    std::cout << "\tQuanity of basic blocks: " << quantity_of_basic_blocks << std::endl;
                    std::string serialized_cfg_addr = cfgs_folder + md5 + ".cfg";
                    double cfg_serialization_delay = 0;
//                    if(md5 == "0ada1119c5dfbbc45cd76cf3a0e1bd43")
//                    double cfg_serialization_delay = Experimentation::serialize_cfg_exp(cfg_exp_results.first, serialized_cfg_addr);


                    std::pair<std::shared_ptr<boost::DFGraph>, double> dfg_inf = Experimentation::get_dfg_exp(cfg_exp_results.first);
                    double dfg_construction_delay = dfg_inf.second;
                    std::cout << "\tDFG Construction delay: " << dfg_construction_delay << std::endl;
                    std::string serialized_dfg_addr = dfgs_folder + md5 + ".dfg";
//                double dfg_serialization_delay = Experimentation::serialize_dfg_exp((*dfg_inf.first), serialized_dfg_addr);
                    double dfg_serialization_delay = 0;
                    int quantity_of_nodes_dfg = dfg_inf.first->vertex_set().size();
                    std::cout << "\tQuanity of nodes in DFG: " << quantity_of_nodes_dfg << std::endl;

                    std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>> graphs = std::make_shared<std::unordered_set<std::shared_ptr<boost::DFGraph>>>();
                    graphs->insert(dfg_inf.first);

                    double feature_extraction_delay = Experimentation::dfg_feature_extraction_for_etgm_delay(md5,
                                                                                                             features_db,
                                                                                                             mnemonic_map,
                                                                                                             graphs);

//                    std::string data_db_insert_query_START = "INSERT INTO batch_whole_arch_run(md5, disassembling_delay,"
//                            " cfg_construction_delay, cfg_serialization_delay, dfg_construction_delay,"
//                            " dfg_serialization_delay, feature_extraction_delay, type_of_features, type_of_extraction,"
//                            " quantity_of_instructions, quantity_of_basic_blocks, quantity_of_nodes_dfg) VALUES (";

                    std::string instance_insert_query = data_db_insert_query_START + "'" + md5 +"','" +
                                                        std::to_string(disassembling_delay) + "','" + std::to_string(cfg_construction_delay) + "','" +
                                                        std::to_string(cfg_serialization_delay) + "','" + std::to_string(dfg_construction_delay) + "','" +
                                                        std::to_string(dfg_serialization_delay) + "','" + std::to_string(feature_extraction_delay) +
                            "','etgm','etgm','" + std::to_string(quantity_of_instructions) + "','" +
                            std::to_string(quantity_of_basic_blocks) + "','" + std::to_string(quantity_of_nodes_dfg) + "');";
//                        std::to_string(dfg_serialization_delay) + ");";

                    int rc;
                    char *zErrMsg = 0;

                    rc = sqlite3_exec(o_db, instance_insert_query.c_str(), NULL, 0, &zErrMsg);
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SasfQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }
                    else{

                        inserted_tuples += 1;


                    }
                }catch (std::exception e){
                    std::cerr << "Error: " << e.what() << std::endl;
                }

            }
        }
        std::string iii = std::to_string(inserted_tuples);
        std::cout << "Inserted: " << iii << std::endl;
    }
    else{
        std::cout  << "ERROR Selection query " << sqlite3_errmsg(i_db) << std::endl;
    }


    sqlite3_close(i_db);
    sqlite3_close(o_db);


}

double Experimentation::dfg_feature_extraction_for_etgm_delay(const std::string &md5, const std::string &db_adddr,
                                                              std::shared_ptr<std::unordered_map<std::string,
                                                                      std::string>> mnemonic_map,
                                                              std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>> graphs) {
    clock_t delay_i = clock();
//
    FeaturesExtraction::extraction_for_etgm(graphs, mnemonic_map, db_adddr, md5);

    clock_t delay_e = clock();
    double delay = double(delay_e - delay_i)/CLOCKS_PER_SEC;
    return delay;
}

double Experimentation::serialize_dfg_exp(boost::DFGraph graph, std::string output) {
    clock_t delay_i = clock();

    std::ofstream o ;
    o.open(output.c_str());
    boost::dynamic_properties dp;

//    dp.property("md5", boost::get(boost::md5_t(), graph));
//    dp.property("mnemonic", boost::get(boost::mnemonic_t(), graph));
//    dp.property("op_str", boost::get(boost::op_str_t(), graph));
//    dp.property("address", boost::get(boost::address_hex_str_t(), graph));
//    dp.property("ins_str", boost::get(boost::ins_str_t(), graph));
//    dp.property("reg_used", boost::get(boost::reg_used_t(), graph));

    boost::write_graphviz(o, graph, boost::make_label_writer(boost::get(boost::mnemonic_t(), graph)));

    clock_t delay_e = clock();
    double delay = double(delay_e - delay_i)/CLOCKS_PER_SEC;
    return delay;
}
