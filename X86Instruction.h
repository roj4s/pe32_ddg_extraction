//
// Created by rojas on 17/10/16.
//

#include <iostream>
#include <unordered_set>
#include <vector>
#include <memory>
#include <capstone/capstone.h>

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_X86INSTRUCTION_H
#define ARCH_UNTIL_FEATURESEXTRACTION_X86INSTRUCTION_H


class X86Instruction {

private:

    int line_number;
    uint16_t size;
    std::string mnemonic;
    std::string op_str;
    std::string address;
    std::shared_ptr<std::unordered_set<std::string>> regs_read;
    std::shared_ptr<std::unordered_set<std::string>> regs_writen;
    std::shared_ptr<std::vector<uint8_t>> bytes;
    std::string str;

public:
    X86Instruction(int line_number, uint16_t size, const std::string &mnemonic, const std::string &op_str,
                   const std::string &address, const std::shared_ptr<std::unordered_set<std::string>> &regs_read,
                   const std::shared_ptr<std::unordered_set<std::string>> &regs_writen,
                   std::shared_ptr<std::vector<uint8_t>> bytes);

    const std::string &getStr() const;

    int getLine_number() const;

    uint16_t getSize() const;

    const std::string &getMnemonic() const;

    const std::string &getOp_str() const;

    const std::string &getAddress() const;

    std::shared_ptr<std::unordered_set<std::string>> getRegs_read() const;

    std::shared_ptr<std::unordered_set<std::string>> getRegs_writen() const;

    std::shared_ptr<std::vector<uint8_t>> getBytes() const;

};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_X86INSTRUCTION_H
