//
// Created by rojas on 25/10/16.
//

#include <unordered_set>
#include "PE32FileInfo.h"


const std::string &PE32FileInfo::getMd5() const {
    return md5;
}

const std::string &PE32FileInfo::getFile_location() const {
    return file_location;
}

const std::string &PE32FileInfo::getEntry_point() const {
    return entry_point;
}

const std::string &PE32FileInfo::getCode_file_location() const {
    return code_file_location;
}

void PE32FileInfo::setMd5(const std::string &md5) {
    PE32FileInfo::md5 = md5;
}

void PE32FileInfo::setFile_location(const std::string &file_location) {
    PE32FileInfo::file_location = file_location;
}

void PE32FileInfo::setEntry_point(const std::string &entry_point) {
    PE32FileInfo::entry_point = entry_point;
}

void PE32FileInfo::setCode_file_location(const std::string &code_file_location) {
    PE32FileInfo::code_file_location = code_file_location;
}

PE32FileInfo::PE32FileInfo(const std::string &db_addr, const std::string &md5): md5(md5), info_source(db_addr)  {


    try{
        std::unordered_map<std::string, std::string> values_map = {{"md5", md5}};
        this->file_location = Aux::select_where_query(db_addr, "exec_info", "file_location", values_map);
        this->entry_point = Aux::select_where_query(db_addr, "exec_info", "entry_point", values_map);
    }
    catch (std::exception e){

        this->code_file_location = "Not found";
        this->entry_point = "Not found";
        std::cerr << "Couldnt create PE32FileInfo Object: " << e.what() << std::endl;

    }


}

PE32FileInfo::PE32FileInfo(const std::string &md5, const std::string &file_location, const std::string &entry_point,
                           const std::string &code_file_location, const std::string &info_source) : md5(md5),
                                                                                                    file_location(
                                                                                                            file_location),
                                                                                                    entry_point(
                                                                                                            entry_point),
                                                                                                    code_file_location(
                                                                                                            code_file_location),
                                                                                                    info_source(
                                                                                                            info_source) {}
