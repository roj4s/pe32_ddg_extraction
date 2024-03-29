//
// Created by rojas on 22/09/16.
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sqlite3.h>
#include <sys/stat.h>
#include <boost/lexical_cast.hpp>
#include <memory>

#ifndef DECISION_TREE_LIKE_INDEXER_AUX_H
#define DECISION_TREE_LIKE_INDEXER_AUX_H



class Aux {

public:
    static std::string get_string_from_file(std::string input_file);
    static int char2int(char input);
    static void hex2bin(const char* src, char * target);
    static std::vector<char> HexToBytes(const std::string& hex);
    static void exec_query_not_return(sqlite3 * con, std::string query);

//    static int * get_random_list_of_numbers(int, int, int);
//    static std::unordered_map<std::string, int> get_random_tuple(std::string, std::string);
//    static std::unordered_set<std::string> get_random_list_of_strings(int, int min_size=5, int max_size=9);
//    static std::string get_random_string_better_approach(const int len=7);
//    static int generate_full_ints_table_with_given_rows_and_column_sizes(int, int, std::string, std::string);
    static int sqlt_callback(void *NotUsed, int argc, char **argv, char **azColName);
//    static std::unordered_set<std::string> get_column_names(std::string db_address, std::string table_name,
//                                                            std::shared_ptr<std::unordered_set<std::string>> exclude_this=std::make_shared<std::unordered_set<std::string>>());
//    static std::vector<std::unordered_map<std::string, std::string>> get_all_tuples(std::string db_addr,
//                                                                                    std::string table_name);
//    static void seek_and_destroy(std::shared_ptr<std::unordered_set<std::string>> search_here, std::shared_ptr<std::unordered_set<std::string>> search_this);
//
//    static std::vector<std::string> from_set_to_vector(std::unordered_set<std::string>);
//
    static std::string select_where_query(std::string db_addr, std::string table_name, std::string select_column_name,
                                          std::unordered_map<std::string, std::string> column_value_where_map);
//
//    static std::vector<std::string> exec_query_get_ordered(std::string db_addr, std::string query);
//    static std::unordered_set<std::string> exec_query_get_unordered(std::string db_addr, std::string query);
//
    static void print_tabs(int tabs=0);
    static std::string t(int tabs=0);
//
//    static double get_porcent(double part, double total);
//
//
//    static int insert_into_db_and_return_id(std::string db, std::string table_name, std::unordered_map<std::string, std::string> col_val);
//
////    static std::vector<std::pair<int, TreeLikeIndex>> sort_tree_index_children_by_key(
////            std::vector<std::pair<int, TreeLikeIndex>>);
//    static bool item_in_vector(std::vector<std::string>, std::string val);
//    static bool item_in_set(std::shared_ptr<std::unordered_set<std::string>>, std::string val);
    static std::string join_strings_vector(std::shared_ptr<std::vector<std::string>> v, std::string separator, std::string scape_with = "");
//    static std::string join_strings_unordered_set(std::shared_ptr<std::unordered_set<std::string>> v, std::string separator,std::string scape_with = "");
    static inline bool file_exists(std::string name);
    static sqlite3 * get_db_connection(std::string db_address);
    static sqlite3 * create_db(std::string db_address, std::string create_statement);
//    static void tabs(int n);
//    static double graph_iso_where_query_time(std::string, std::string, std::unordered_map<std::string, int>);
//    static double subgraph_iso_where_query_time(std::string, std::string, std::unordered_map<std::string, int>);
//    static double supergraph_iso_where_query_time(std::string, std::string, std::unordered_map<std::string, int>);
    static std::string get_time_sufix();
//    static std::string get_one_table_name_from_db(std::string db_address);
//    static long get_file_size(std::string file_address);
//    static std::string sep();
    static void mkdir(std::string path, bool notify=false, std::string tag="NoTag");
//    static int get_one_table_size(std::string db_addr, std::string table_name);
//    static int insert_indexer_test_session(std::unordered_map<std::string, std::string> vals, std::string db);
//    static std::unordered_map<std::string, std::string> get_indexer_test_session_fields_mapping();
    static int get_int_from_hex_string(const std::string &hex_string);
};

#endif //DECISION_TREE_LIKE_INDEXER_AUX_H