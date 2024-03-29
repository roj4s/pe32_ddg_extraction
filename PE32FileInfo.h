//
// Created by rojas on 25/10/16.
//
#include <iostream>
#include "Aux.h"

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_PE32FILEINFO_H
#define ARCH_UNTIL_FEATURESEXTRACTION_PE32FILEINFO_H


class PE32FileInfo {

private:
    std::string md5;
    std::string file_location;
    std::string entry_point;
    std::string code_file_location;
    std::string info_source;

public:
    PE32FileInfo(const std::string &md5, const std::string &file_location, const std::string &entry_point,
                 const std::string &code_file_location, const std::string &info_source);

    PE32FileInfo(const std::string &db_addr, const std::string &md5);

    const std::string &getMd5() const;

    const std::string &getFile_location() const;

    const std::string &getEntry_point() const;

    const std::string &getCode_file_location() const;

    void setMd5(const std::string &md5);

    void setFile_location(const std::string &file_location);

    void setEntry_point(const std::string &entry_point);

    void setCode_file_location(const std::string &code_file_location);


};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_PE32FILEINFO_H
