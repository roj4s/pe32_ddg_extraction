//
// Created by rojas on 17/10/16.
//

#include "X86Instruction.h"

X86Instruction::X86Instruction(int line_number, uint16_t size, const std::string &mnemonic, const std::string &op_str,
                               const std::string &address,
                               const std::shared_ptr<std::unordered_set<std::string>> &regs_read,
                               const std::shared_ptr<std::unordered_set<std::string>> &regs_writen,
                               std::shared_ptr<std::vector<uint8_t>> bytes)
        : line_number(line_number), size(size), mnemonic(mnemonic), op_str(op_str), address(address),
          regs_read(regs_read), regs_writen(regs_writen), bytes(bytes) {

        this->str = address + ": " + mnemonic + " " + op_str;

}

int X86Instruction::getLine_number() const {
    return line_number;
}

uint16_t X86Instruction::getSize() const {
    return size;
}


std::shared_ptr<std::vector<uint8_t>> X86Instruction::getBytes() const {
    return bytes;
}

const std::string &X86Instruction::getMnemonic() const {
    return mnemonic;
}

const std::string &X86Instruction::getOp_str() const {
    return op_str;
}

const std::string &X86Instruction::getAddress() const {
    return address;
}

std::shared_ptr<std::unordered_set<std::string>> X86Instruction::getRegs_read() const {
    return regs_read;
}

std::shared_ptr<std::unordered_set<std::string>> X86Instruction::getRegs_writen() const {
    return regs_writen;
}

const std::string &X86Instruction::getStr() const {
    return str;
}


