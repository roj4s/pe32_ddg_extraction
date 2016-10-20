//
// Created by rojas on 13/10/16.
//

#include "Experimentation.h"
#include "Disassembler.h"

std::pair<std::shared_ptr<DisassemblingInfo>, double> Experimentation::disassembling_exp(std::string code_file_addr) {

    clock_t delay_i = clock();
    std::shared_ptr<DisassemblingInfo> inf = Disassembler::dissasemble(code_file_addr);
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
