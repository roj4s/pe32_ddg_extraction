//
// Created by rojas on 22/09/16.
//

#include "Aux.h"
#include <iostream>

//static std::vector<std::string> * exec_query_get_ordered(std::string db_addr, std::string table_name, std::string query){
//
//	sqlite3* con = Aux::get_db_connection(db_addr);
//    	sqlite3_stmt * statement;
//
//	static std::vector<std::string> * results = new std::vector<std::string>();
//
//	sqlite3_prepare(con, query.c_str(), -1, &statement, 0);
//
//    while(sqlite3_step(statement) == SQLITE_ROW)
//        results->push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement,0)));
//
//	return results;
//
//
//}
//
//static std::unordered_set<std::string> * exec_query_get_unordered(std::string db_addr, std::string table_name, std::string query){
//
//	std::vector<std::string> vals = Aux::exec_query_get_ordered(db_addr,query);
//	static std::unordered_set<std::string> * results = new std::unordered_set<std::string>();
//	results->insert(vals.begin(), vals.end());
//
//    return results;
//
//}


std::string Aux::get_string_from_file(std::string input_file) {

    std::ifstream _f(input_file);
    std::string to_return = "";
    if(_f.is_open()){

        std::string line;
        while(getline(_f,line ))
            to_return += line;

        _f.close();

    }

    else
        throw "Couldn't open file.";

    return to_return;

}

std::vector<char> Aux::HexToBytes(const std::string& hex) {
    std::vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
//        std::cout << byteString << std::endl;
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

int Aux::char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

void Aux::hex2bin(const char* src, char* target)
{
    while(*src && src[1])
    {
        *(target++) = char2int(*src)*16 + char2int(src[1]);
        src += 2;
    }
}

//
//double Aux::graph_iso_where_query_time(std::string db, std::string table_name, std::unordered_map<std::string,
//        int> _tuple)
//{
//
//    sqlite3* con = Aux::get_db_connection(db);
//    sqlite3_stmt * statement;
//    std::string query = "SELECT rowid FROM " + table_name + " WHERE ";
//
//    bool first = true;
//    for(std::pair<std::string, int> p: _tuple)
//    {
//        if(!first) query += " AND ";
//        first = false;
//        query += p.first + " = " + std::to_string(p.second);
//    }
//
//    query += " ;";
//
////cout << "WHERE QUERY IS: " << query << endl;
//
//    clock_t c_i = clock();
//    sqlite3_prepare(con, query.c_str(), -1, &statement, 0);
//    int row_id = -1;
//    while(sqlite3_step(statement) == SQLITE_ROW)
//        row_id = sqlite3_column_int(statement,0);
//    clock_t c_e = clock();
//    sqlite3_finalize(statement);
//    double final_time = double(c_e - c_i)/CLOCKS_PER_SEC;
//    std::cout << "Rowid is: " << row_id << std::endl;
////cout << "Time querying was: " << final_time << endl;
//    return final_time;
//
//}
//
//double Aux::subgraph_iso_where_query_time(std::string db, std::string table_name, std::unordered_map<std::string,
//        int> _tuple)
//{
//
//    sqlite3* con = Aux::get_db_connection(db);
//    sqlite3_stmt * statement;
//    std::string query = "SELECT rowid FROM " + table_name + " WHERE ";
//
//    bool first = true;
//    for(std::pair<std::string, int> p: _tuple)
//    {
//        if(!first) query += " AND ";
//        first = false;
//        query += p.first + " <= " + std::to_string(p.second);
//    }
//
//    query += " ;";
//
////cout << "WHERE QUERY IS: " << query << endl;
//
//    clock_t c_i = clock();
//    sqlite3_prepare(con, query.c_str(), -1, &statement, 0);
//    int row_id = -1;
//    while(sqlite3_step(statement) == SQLITE_ROW)
//        row_id = sqlite3_column_int(statement,0);
//    clock_t c_e = clock();
//    sqlite3_finalize(statement);
//    double final_time = double(c_e - c_i)/CLOCKS_PER_SEC;
//    std::cout << "Rowid is: " << row_id << std::endl;
////cout << "Time querying was: " << final_time << endl;
//    return final_time;
//
//}
//
//double Aux::supergraph_iso_where_query_time(std::string db, std::string table_name, std::unordered_map<std::string,
//        int> _tuple)
//{
//
//    sqlite3* con = Aux::get_db_connection(db);
//    sqlite3_stmt * statement;
//    std::string query = "SELECT rowid FROM " + table_name + " WHERE ";
//
//    bool first = true;
//    for(std::pair<std::string, int> p: _tuple)
//    {
//        if(!first) query += " AND ";
//        first = false;
//        query += p.first + " >= " + std::to_string(p.second);
//    }
//
//    query += " ;";
//
////cout << "WHERE QUERY IS: " << query << endl;
//
//    clock_t c_i = clock();
//    sqlite3_prepare(con, query.c_str(), -1, &statement, 0);
//    int row_id = -1;
//    while(sqlite3_step(statement) == SQLITE_ROW)
//        row_id = sqlite3_column_int(statement,0);
//    clock_t c_e = clock();
//    sqlite3_finalize(statement);
//    double final_time = double(c_e - c_i)/CLOCKS_PER_SEC;
//    std::cout << "Rowid is: " << row_id << std::endl;
////cout << "Time querying was: " << final_time << endl;
//    return final_time;
//
//}
//
//std::unordered_map<std::string, int> Aux::get_random_tuple(std::string source_db, std::string table)
//{
//
//    std::unordered_set<std::string> columns_names = Aux::get_column_names(source_db, table);
//    std::string query_count = "SELECT count(*) FROM " + table;
//
//    int table_count = 0;
//    sqlite3* con = Aux::get_db_connection(source_db);
//    sqlite3_stmt * statement;
//    if(sqlite3_prepare(con, query_count.c_str(),-1,&statement,0) == SQLITE_OK)
//        if(sqlite3_step(statement) == SQLITE_ROW)
//            table_count = sqlite3_column_int(statement, 0);
//
//    std::cout << "Table count is: " << table_count << std::endl;
//
//    bool first = true;
//
//    std::string query = "SELECT ";
//    for(std::string col: columns_names)
//    {
//        if(!first) query+= ",";
//        first = false;
//        query += col;
//    }
//
//    int random_id = Aux::get_random_list_of_numbers(1,0,table_count)[0];
//    std::cout << "Random id is: " << random_id << std::endl;
//    query+=" FROM " + table + " WHERE rowid = " + std::to_string(random_id) + ";";
//
//    sqlite3_prepare(con, query.c_str(), -1, &statement, 0);
//    sqlite3_step(statement);
//    std::unordered_map<std::string, int> _tuple;
//    int i=0;
//    for(std::string col: columns_names)
//    {
////        std::pair<std::string, std::string> p(col, std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement,i))));
//        std::pair<std::string, int> p(col, sqlite3_column_int(statement,i));
//        _tuple.insert(p);
//        i++;
//    }
//    sqlite3_finalize(statement);
//
//    return _tuple;
//}
//
//void Aux::tabs(int n)
//{
//    for(int i=0;i<=n;i++) std::cout << "\t" ;
//}
//
sqlite3 * Aux::get_db_connection(std::string db_address)
{
    sqlite3 * db;
    int rc;

    if(!Aux::file_exists(db_address))
    {
        std::string msg = "Db address don't exists. Db address: " + db_address;
        std::cout << msg << std::endl;
        return 0;
    }
    rc = sqlite3_open(db_address.c_str(), &db);



    if(rc)
    {
        fprintf(stderr, "Error opening db: %s\n", sqlite3_errmsg(db));
        std::cout<< "DB is: " << db_address << std::endl;
        return 0;
    }
    return db;
}
//
sqlite3 * Aux::create_db(std::string db_address, std::string create_statement)
{
    sqlite3 * db;
    int rc;
    char * zErrMsg;

    rc = sqlite3_open(db_address.c_str(), &db);
//    std::cout << "Will create table Statement is: " << create_statement << std::endl;
    if(rc)
    {
        fprintf(stderr, "Error opening db: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_exec(db, create_statement.c_str(), Aux::sqlt_callback, 0, &zErrMsg);
    if(rc != SQLITE_OK) std::cout << "Error creating table: " << zErrMsg << std::endl;
    else
    {
//        fprintf(stderr, "Table created successfully.");
        return db;
    }
    return 0;
}
//
inline bool Aux::file_exists(std::string name) {
    std::ifstream f(name.c_str());
    return f.good();
}
//
int Aux::sqlt_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
//    for(int i=0; i<argc; i++)
//    {
//        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]: "NULL");
//    }
//    printf("\n");
    return 0;
}

void Aux::exec_query_not_return(sqlite3 *con, std::string query) {

    sqlite3_stmt * stmnt;
    sqlite3_prepare(con, query.c_str(), -1, &stmnt, NULL);
    sqlite3_step(stmnt);

}

//
//std::unordered_set<std::string> Aux::get_column_names(std::string db_address, std::string table_name,
//        std::shared_ptr<std::unordered_set<std::string>> exclude_this)
//{
//
//    sqlite3 * db = Aux::get_db_connection(db_address);
//    std::string query = "PRAGMA table_info (" + table_name + ");";
//    sqlite3_stmt * stmnt;
//
//    std::unordered_set<std::string> columns_names = {};
//
//
//    if(sqlite3_prepare(db, query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
//    {
//        int res = 0;
//        while(1){
//            res = sqlite3_step(stmnt);
//            if(res == SQLITE_ROW)
//            {
//                std::string column = (char*)sqlite3_column_text(stmnt,1);
//                if(! Aux::item_in_set(exclude_this, column))
//                    columns_names.insert(column);
//            }
//            if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
//        }
//    }
//
//    return columns_names;
//}
//
//int * Aux::get_random_list_of_numbers(int size_of_list, int initial_num, int end_num)
//{
//    int r[size_of_list];
//
//    std::random_device rd;     // only used once to initialise (seed) engine
//    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
//    std::uniform_int_distribution<int> uni(initial_num, end_num); // guaranteed unbiased
//
//    for(int i =0;i < size_of_list; i++)
//    {
//        r[i] = uni(rng);
//    }
//
//    int * address_to_return = r;
//    return address_to_return;
//}
//
//std::unordered_set<std::string> Aux::get_random_list_of_strings(int size_of_list, int min_size, int max_size)
//{
////    static const char ascii_letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//    static const char ascii_letters[] = "abcdefghijklmnopqrstuvwxyz";
////    short int len_of_ascii_letters = 52;
//    short int len_of_ascii_letters = 26;
//    std::unordered_set<std::string> strings;
//    for(int i=0;i<size_of_list;i++)
//    {
//        int random_size_for_name = Aux::get_random_list_of_numbers(1,min_size,max_size)[0];
//        std::string name = "";
//        generate_again:
//        for(int j=0; j < random_size_for_name; j++)
//        {
//            int random_index_for_char = Aux::get_random_list_of_numbers(1,0,len_of_ascii_letters)[0];
//            name+= *(ascii_letters + random_index_for_char);
//        }
//        if(name.size() == 0 || strings.insert(name).second==0)
//            goto generate_again;
//    }
//    return strings;
//}
//
//int Aux::generate_full_ints_table_with_given_rows_and_column_sizes(int row_size, int column_size,std::string address,
//                                                                   std::string table_name)
//{
//
//    std::string db_location = address+"/"+table_name+".db";
//
//    std::string statements = "PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; BEGIN TRANSACTION; CREATE TABLE " + table_name + " (rowid INTEGER PRIMARY KEY,";
//    std::unordered_set<std::string> column_names = Aux::get_random_list_of_strings(column_size);
//    std::string columns_string = "";
//    int i=0;
//    for(std::string col:column_names)
//    {
//        statements+= col + " INTEGER";
//        columns_string+=col;
//        if(i != column_size -1)
//        {
//            statements+=",";
//            columns_string+= ",";
//        }
//        i++;
//    }
//
//    statements += ");\n";
//
//    for(int i=0; i < row_size; i++)
//    {
//
//        int * list_of_random_numbers = Aux::get_random_list_of_numbers(column_size, 0, 1000);
//        std::string values = "";
//
//        for(int j=0; j < column_size - 1 ; j++) values += std::to_string(*(list_of_random_numbers + j)) + ",";
//
//        values += std::to_string(*(list_of_random_numbers + column_size - 1));
//
//        statements += "INSERT INTO " + table_name + "(" + columns_string + ")" + " VALUES (" + values + ");\n";
//    }
//// "CREATE INDEX index_rowid ON {table}(rowid)"
//    statements += "CREATE INDEX indx_rowid ON " + table_name + "(rowid);\n";
//    statements += "END TRANSACTION;";
//    //cout << "Generating random tuples delay: " << generating_random_tuple_time/CLOCKS_PER_SEC << endl;
////   cout << "Adding insert statementes time was: " << adding_insert_statements_to_main_statement_time/CLOCKS_PER_SEC << endl;
//    clock_t execution_time_i = clock();
//    Aux::create_db(db_location,statements);
////   rc = sqlite3_exec(db, statements.c_str(), Aux::sqlt_callback, 0, &zErrMsg);
//    clock_t execution_time_e = clock();
//    double delay_executing_transaction = double(execution_time_e - execution_time_i)/CLOCKS_PER_SEC;
//    std::cout << "Delay executing transaction: " << delay_executing_transaction << std::endl;
///*   if(rc != SQLITE_OK)
//   {
//    fprintf(stderr, "Error, executing query : %s\n", zErrMsg);
//    sqlite3_free(zErrMsg);
//   }
//   else
//   {
//    fprintf(stderr, "Query executed successfully");
//   }
//   sqlite3_close(db);*/
//    return 0;
//
//}
//
std::string Aux::get_time_sufix() {

    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"_%a_%b_%F_%X",timeinfo);
    puts (buffer);

    return std::string(buffer);
}
//
//bool Aux::item_in_vector(std::vector<std::string> v, std::string val) {
//    return std::find(v.begin(), v.end(), val) != v.end();
//}
//
//bool Aux::item_in_set(std::shared_ptr<std::unordered_set<std::string>> v, std::string val) {
//    return std::find(v->begin(), v->end(), val) != v->end();
//}
//
std::string Aux::join_strings_vector(std::shared_ptr<std::vector<std::string>> v, std::string separator, std::string scape_with) {

    std::string joined = "";
    bool add_separator = false;

    for(std::vector<std::string>::iterator it = v->begin(); it != v->end(); ++it){

        if(add_separator)
            joined += separator;

        joined += scape_with + *it + scape_with;
        add_separator = true;
    }

    return joined;
}
//
//std::string Aux::join_strings_unordered_set(std::shared_ptr<std::unordered_set<std::string>> v, std::string separator,
//                                            std::string scape_with) {
//    std::string joined = "";
//    bool add_separator = false;
//
//    for(std::unordered_set<std::string>::iterator it = v->begin(); it != v->end(); ++it){
//
//        if(add_separator){
//            joined += separator;
//        }
//        joined += scape_with + *it + scape_with;
//        add_separator = true;
//    }
//
//    return joined;
//}
//
////std::vector<std::pair<int, TreeLikeIndex>> Aux::sort_tree_index_children_by_key(std::vector<std::pair<int,
////        TreeLikeIndex>> to_sort) {
////
////    std::vector<int> keys{};
////    std::unordered_map<int, TreeLikeIndex> map = {};
////    for(std::vector<std::pair<int, TreeLikeIndex>>::iterator it = to_sort.begin(); it != to_sort.end(); ++it){
////
////        std::pair<int, TreeLikeIndex> _pair = *(it);
////        keys.push_back(_pair.first);
////        map[_pair.first] = _pair.second;
////
////    }
////
////    std::sort(keys.begin(), keys.end());
////
////    std::vector<std::pair<int, TreeLikeIndex>> to_ret;
////
////    for(int v: keys)
////        to_ret.push_back(std::make_pair(v, map.at(v)));
////
////    return  to_ret;
////
////}
//
//std::string Aux::get_one_table_name_from_db(std::string db_address) {
//
//    sqlite3 * db = Aux::get_db_connection(db_address);
//    std::string query = "select tbl_name from sqlite_master where type = 'table' limit 1";
//    sqlite3_stmt * stmnt;
//    std::string table_name = "";
//
//    if(sqlite3_prepare(db, query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
//    {
//
//        int res = 0;
//        while(1){
//            res = sqlite3_step(stmnt);
//            if(res == SQLITE_ROW)
//            {
//                table_name = reinterpret_cast<const char*>(sqlite3_column_text(stmnt,0));
//            }
//            if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
//        }
//    }
//
//    return table_name;
//}
//
//long Aux::get_file_size(std::string filename)
//{
//    struct stat stat_buf;
//    int rc = stat(filename.c_str(), &stat_buf);
//    return rc == 0 ? stat_buf.st_size : -1;
//}
//
//std::string Aux::sep() {
//  #ifdef _WIN32
//    return "\\";
//  #else
//    return "/";
//  #endif
//}
//
void Aux::mkdir(std::string path, bool notify, std::string tag) {

    std::string command = "mkdir " + path;
    std::system(command.c_str());

}
//
//int Aux::get_one_table_size(std::string db_addr, std::string table_name) {
//    sqlite3 * db = Aux::get_db_connection(db_addr);
//    std::string query = "select count(*) from " + table_name;
//    sqlite3_stmt * stmnt;
//    int count = -1;
//
//    if(sqlite3_prepare(db, query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
//    {
//
//        int res = 0;
//        while(1){
//            res = sqlite3_step(stmnt);
//            if(res == SQLITE_ROW)
//            {
//                count = sqlite3_column_int(stmnt,0);
//                break;
//            }
//            if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
//        }
//    }
//
//    return count;
//}
//
//
//
//int Aux::insert_indexer_test_session(std::unordered_map<std::string, std::string> vals, std::string db) {
//    /*Insert a indexer test session on specified db and return the id of the inserted tuple.
//     *
//     * */
//
//    int id = -1;
//    std::vector<std::string> test_session_fields_mapping;
//
//    return id;
//}
//
//std::unordered_map<std::string, std::string> Aux::get_indexer_test_session_fields_mapping() {
//
//    std::unordered_map<std::string, std::string> indexer_test_session_mapping = {
//        {"indexer_creation_delay", "float(10)"},
//        {"indexer_serialization_delay", "float(10)"},
//        {"_date", "varchar(255)"},
//        {"train_db_address", "varchar(255)"},
//        {"test_db_address", "varchar(255)"},
//        {"train_table_name", "varchar(255)"},
//        {"md5_column_name", "varchar(255)"},
//        {"seed_column_name", "varchar(255)"},
//        {"output_folder", "varchar(255)"}
//    };
//
//    return indexer_test_session_mapping;
//
//}
//
//int Aux::insert_into_db_and_return_id(std::string db_addr, std::string table_name, std::unordered_map<std::string,
//        std::string> col_val) {
//
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//
//    db = Aux::get_db_connection(db_addr);
//
//    /* Create SQL statement */
//
//    std::string insert_q = "INSERT INTO " + table_name + " (";;
//    std::vector<std::string> fields = {};
//
//    for(std::unordered_map<std::string,std::string>::iterator it = col_val.begin(); it != col_val.end(); ++it){
//        std::pair<std::string, std::string> info = *(it);
//        fields.push_back(info.first);
//    }
//
//    long columns_q = fields.size();
//
//
//    for (int k = 0; k < columns_q; k++) {
//        insert_q += fields[k];
//        if(k < columns_q - 1)
//            insert_q += ",";
//        else
//            insert_q += ") VALUES (";
//    }
//
//    for(int i=0; i< columns_q; i++){
//
//        std::string key = fields[i];
//        insert_q += "'" + col_val[key] + "'";
//        if(i< columns_q - 1)
//            insert_q += ",";
//        else
//            insert_q += ");";
//    }
////    std::cout << insert_q << std::endl;
//
//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, insert_q.c_str(), NULL, 0, &zErrMsg);
//    if( rc != SQLITE_OK ){
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    }
////    else{
////        fprintf(stdout, "Records created successfully\n");
////    }
//
//
//    int id = sqlite3_last_insert_rowid(db);
//    sqlite3_close(db);
//    return id;
//
//}
////
////std::string Aux::to_str(auto el) {
////    return boost::lexical_cast<std::string>(el);
////}
//
//std::vector<std::unordered_map<std::string, std::string>> Aux::get_all_tuples(std::string db_addr, std::string table_name) {
//
//    std::unordered_set<std::string> columns_names_set = Aux::get_column_names(db_addr, table_name);
//
//    std::vector<std::string> ordered_columns_names = Aux::from_set_to_vector(columns_names_set);
//    std::shared_ptr<std::vector<std::string>> els_ptr = std::make_shared<std::vector<std::string>>(ordered_columns_names);
//
//    std::string columns_str = Aux::join_strings_vector(els_ptr, ",");
//
//    std::string select_query = "select " + columns_str + " from " + table_name;
//
//    sqlite3 * db = Aux::get_db_connection(db_addr);
//    sqlite3_stmt * stmnt;
//
//    std::vector<std::unordered_map<std::string, std::string>> tuples = {};
//
//    if(sqlite3_prepare(db, select_query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
//    {
//
//
//        int res = 0;
//        while(1){
//            res = sqlite3_step(stmnt);
//            if(res == SQLITE_ROW)
//            {
//                std::unordered_map<std::string, std::string> row = {};
//                int column_index = 0;
//                for(std::string column : ordered_columns_names){
//
//                    row[column] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmnt, column_index)));
//                    column_index ++;
//                }
//
//                tuples.push_back(row);
//            }
//            if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
//        }
//
//    }
//    return tuples;
//}
//
//std::vector<std::string> Aux::from_set_to_vector(std::unordered_set<std::string> _set) {
//    std::vector<std::string> vals = {};
//    for(std::string val: _set)
//        vals.push_back(val);
//
//    return vals;
//
//}
//
//double Aux::get_porcent(double part, double total) {
//    return (part/total) * 100;
//}
//
std::string Aux::select_where_query(std::string db_addr, std::string table_name, std::string select_column_name,
                                    std::unordered_map<std::string, std::string> column_value_where_map) {

    sqlite3 * db = Aux::get_db_connection(db_addr);
    std::string select_query = "select " + select_column_name + " from " + table_name + " where ";

    std::vector<std::string> conditions = {};

    for(std::unordered_map<std::string, std::string>::iterator it = column_value_where_map.begin();
        it != column_value_where_map.end(); ++it){
        std::string condition = it->first + "='" + it->second + "'";
        conditions.push_back(condition);
    }

    std::shared_ptr<std::vector<std::string>> conditions_ptr = std::make_shared<std::vector<std::string>>(conditions);
    std::string conditions_str = Aux::join_strings_vector(conditions_ptr, " AND ");

    select_query += conditions_str + " LIMIT 1;";

//    std::cout << "Selection query is: " << select_query << std::endl;
    std::string result = "";
    sqlite3_stmt *stmnt;

    if(sqlite3_prepare(db, select_query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
    {


        int res = 0;
        while(1){
            res = sqlite3_step(stmnt);
            if(res == SQLITE_ROW)
                result = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmnt, 0)));

            if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
        }

    }
    else{
        std::cout  << "ERROR Selection query " << sqlite3_errmsg(db) << std::endl;
    }


    sqlite3_close(db);

    return result;

}

 int Aux::get_int_from_hex_string(const std::string &hex_string) {

    int x;

    std::stringstream ss;
    ss << std::hex << hex_string;
    ss >> x;
    return x;

}

std::string Aux::t(int tabs) {

    std::string tt = "";
    for(int i=0;i<tabs; i++) tt+="\t";
    return tt;

}
//
//std::string Aux::get_random_string_better_approach(const int len) {
//
//    std::string name = "";
//    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
//
//    for (int i = 0; i < len; ++i)
//        name += alphanum[rand() % (sizeof(alphanum) - 1)];
//
//    return name;
//}
//
//std::vector<std::string> Aux::exec_query_get_ordered(std::string db_addr, std::string query) {
//
//    sqlite3 * db = Aux::get_db_connection(db_addr);
//    sqlite3_stmt *stmnt;
//
//    std::vector<std::string> results = std::vector<std::string>();
//
//    if(sqlite3_prepare(db, query.c_str(), -1, &stmnt, 0) == SQLITE_OK)
//    {
//
//
//        int res = 0;
//        while(1){
//            res = sqlite3_step(stmnt);
//            if(res == SQLITE_ROW)
//            {
//                std::string val = reinterpret_cast<const char *>(sqlite3_column_text(stmnt, 0));
//                std::cerr<<"Val in query is: " << val << std::endl;
//                results.push_back(val);
//            }
//            if ( res == SQLITE_DONE || res==SQLITE_ERROR) break;
//        }
//
//    }
//
//    std::cerr<< "After filling results is: ";
//    for(std::string v: results)
//        std::cerr<<v<<",";
//    std::cerr<<std::endl;
//
//    return results;
//}
//
//std::unordered_set<std::string>
//Aux::exec_query_get_unordered(std::string db_addr, std::string query) {
//
//    std::unordered_set<std::string> results ;
//    std::vector<std::string> vals = Aux::exec_query_get_ordered(db_addr, query);
//    results.insert(vals.begin(), vals.end());
//    return results;
//
//}
//
//void Aux::print_tabs(int tabs) {
//
//    for(int i=0;i<tabs;i++)
//        std::cout<<"\t";
//
//}
//
//void Aux::seek_and_destroy(std::shared_ptr<std::unordered_set<std::string>> search_here, std::shared_ptr<std::unordered_set<std::string>> search_this) {
//
//    for(std::unordered_set<std::string>::iterator search_here_it = search_here->begin(); search_here_it!= search_here->end(); ++search_here_it)
//        for(std::unordered_set<std::string>::iterator search_this_it = search_this->begin(); search_this_it != search_this->end(); ++search_this_it)
//            if(*search_here_it == *search_this_it){
//                std::cout << "Deleting " << *search_this_it << std::endl;
//                search_here->erase(search_here_it);
//            }
//
//}
//
//
//int generate_many_tables_main(std::string directory_address)
//{
//    int tables_sizes_count = 8;
//    int features_space_count = 6;
//    int tables_sizes[] = {1000, 10000, 50000, 100000, 500000, 1000000, 1500000, 2000000};
//    int feature_space_sizes[] = {5, 10, 15, 20, 25, 30};
//    for(int i=0; i< tables_sizes_count; i++)
//    {
//        for(int j=0; j<features_space_count; j++)
//        {
//            int row_size = tables_sizes[i];
//            int column_size = feature_space_sizes[j];
//            std::string table_name = "t_" + std::to_string(row_size) + "_" + std::to_string(column_size);
//            Aux::generate_full_ints_table_with_given_rows_and_column_sizes(row_size,column_size,directory_address,
//                                                                           table_name);
//        }
//    }
//    return 0;
//}
//
///*int main()
//{
//
//string db_addr = "./tables/t_1000_15.db";
//string table_name = "t_1000_15";
//unordered_set<string> cn = Aux::get_column_names(db_addr, table_name);
//for_each(cn.begin(), cn.end(), [](string v){cout << v << endl;});
//return 0;
//}
//
//
///*int main()
//{
//  int *p;
//  int _size = 100;
//  int initial_num = 2;
//  int end_num = 50;
//  //p = Aux::get_random_list_of_numbers(_size, initial_num, end_num);
//  clock_t initial_time = clock();
//  char ** generated_strings = Aux::get_random_list_of_strings(_size);
//  //sleep(1);
//  clock_t end_time = clock();
//  cout << "Generated: " << _size << " strings in: " << double(end_time - initial_time)/CLOCKS_PER_SEC << " seconds." << endl;
//  cout << "Time before was: " << initial_time << " and time after was: " <<end_time << endl;
// return 0;
//}*/

