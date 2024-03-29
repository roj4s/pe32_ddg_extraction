//
// Created by rojas on 11/10/16.
//



#ifndef ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H
#define ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H


#include <unordered_set>
#include <queue>
#include "CustomBasicBlockImpl.h"
#include "deps/json.hpp"
#include <fstream>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>

using json = nlohmann::json ;


namespace boost{

    struct instructions{
        typedef vertex_property_tag kind;
    };

//    struct md5_t{
//        typedef graph_property_tag kind;
//    };

    typedef property<instructions, std::string> CFGVertexProperties;

//    typedef property<md5_t, std::string> GraphProperties;

//    typedef adjacency_list<listS, listS, bidirectionalS, VertexProperties> DFGraph;
//    typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties, GraphProperties> DFGraph;
    typedef adjacency_list<vecS, vecS, bidirectionalS, CFGVertexProperties> CFG;

    typedef graph_traits<CFG> CFGTraits;
    typedef CFGTraits::vertex_descriptor  CFGVertex;
    typedef CFGTraits::vertex_iterator CFGVertexIterator;
    typedef CFGTraits::edge_descriptor CFGEdge;
    typedef property_map<CFG, instructions>::type CFGInstructionsMap;
//    typedef property_map<DFGraph, md5_t>::type DFGMd5;

}

class CustomCFGImpl {

private:
    std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<CustomBasicBlockImpl>>> basic_blocks;
    std::string entry_point;
    std::shared_ptr<std::unordered_set<std::string>> visited_set;
    std::shared_ptr<std::unordered_set<std::string>> non_existent_addresses;
    std::shared_ptr<std::unordered_set<std::string>> call_return_addresses;
    std::string md5;
public:
    const std::string &getMd5() const;

public:
    CustomCFGImpl(
            const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>> &basic_blocks,
            const std::string &entry_point, const std::shared_ptr<std::unordered_set<std::string>> &visited_set,
            const std::shared_ptr<std::unordered_set<std::string>> &non_existent_addresses,
            const std::shared_ptr<std::unordered_set<std::string>> &call_return_addresses, std::string md5);

    int serialize(std::string destination_address, int identation_size =4);

    json get_bb_json(std::string from_this_address,  std::shared_ptr<std::unordered_map<std::string, json>> visited_map);

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CustomBasicBlockImpl>>>
    getBasic_blocks() const;

    std::string get_json_string(std::string entry_point, std::string previous_str="", int depht=0);

    std::shared_ptr<boost::CFG> get_cfg_as_boost();

    void recursively_fill_cfg(std::string entry_point, std::shared_ptr<boost::CFG> cfg,
                              std::shared_ptr<std::unordered_map<std::string, boost::CFGVertex>> visited_map,  std::string from="");


    const std::string &getEntry_point() const;

    std::shared_ptr<std::unordered_set<std::string>> getVisited_set() const;

    std::shared_ptr<std::unordered_set<std::string>> getNon_existent_addresses() const;

    std::shared_ptr<std::unordered_set<std::string>> getCall_return_addresses() const;

    int write_graphiz(std::string output_addr);
};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_CUSTOMCFGIMPL_H
