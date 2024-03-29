//
// Created by rojas on 20/10/16.
//

#include "FeaturesExtraction.h"
#include "Aux.h"

void
FeaturesExtraction::extraction_for_etgm(std::shared_ptr<std::unordered_set<std::shared_ptr<boost::DFGraph>>> graphs,
                                        std::shared_ptr<std::unordered_map<std::string, std::string>> mnemonic_classification,
                                        std::string dest_db_address, std::string md5) {

    std::vector<std::string> ordered_keys;
    std::string creation_query = "CREATE TABLE IF NOT EXISTS  extraction_for_etgm(md5 varchar,";
    std::string insert_query = "INSERT INTO extraction_for_etgm(md5,";
    std::unordered_set<std::string> columns;
    bool add_comma = false;

    for(std::unordered_map<std::string, std::string>::iterator clas_it = mnemonic_classification->begin();
        clas_it != mnemonic_classification->end(); ++clas_it) {

        if(!columns.insert(clas_it->second).second)
            continue;

        if(add_comma){
            creation_query += ",";
            insert_query += ",";
        }

        std::string column = "ft_" + clas_it->second;
        creation_query += column + " INTEGER";
        ordered_keys.push_back(clas_it->second);
        insert_query += column;
        add_comma = true;
    }
    creation_query += ");";

    sqlite3 * conn = Aux::create_db(dest_db_address, creation_query);
    insert_query += ") VALUES  ('" + md5 + "',";

    for(std::unordered_set<std::shared_ptr<boost::DFGraph>>::iterator g_it = graphs->begin();
        g_it != graphs->end(); ++g_it){

        boost::DFGraph this_graph = **g_it;
        std::unordered_map<std::string, int> property_count_mapping;
        boost::DFGVertexIterator vi, ve;
        boost::tie(vi, ve) = boost::vertices(this_graph);
        boost::DFGVertex v;
        boost::DFGVMnemonicMap mnemonicMap = boost::get(boost::mnemonic_t(), this_graph);

        while(vi != ve)
        {
            std::string mnemonic = mnemonicMap[*vi];
            std::string classification = mnemonic_classification->at(mnemonic);

            std::unordered_map<std::string, int>::iterator found = property_count_mapping.find(classification);
            if(found != property_count_mapping.end()){
                int old_val = found->second;
                int new_val = old_val + 1;
                property_count_mapping[classification] = new_val;

            }
            else{

                property_count_mapping[classification] = 1;

            }
            vi++;
        }
        std::string this_graph_insert_query = insert_query;
        add_comma = false;
        for(std::string k: ordered_keys){

            std::string val = "";
            std::unordered_map<std::string, int>::iterator found = property_count_mapping.find(k);
            if(found != property_count_mapping.end()){
                val = std::to_string(found->second);

            }
            else
                val = "0";
            if(add_comma)
                this_graph_insert_query += ",";

            this_graph_insert_query += "'" + val + "'";
            add_comma = true;

        }

        this_graph_insert_query += ");";

        int rc;
        char *zErrMsg = 0;

        rc = sqlite3_exec(conn, this_graph_insert_query.c_str(), NULL, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }


//        Aux::exec_query_not_return(conn, this_graph_insert_query);

    }
    sqlite3_close(conn);
}
