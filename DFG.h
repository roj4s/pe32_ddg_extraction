//
// Created by rojas on 17/10/16.
//

#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>

#ifndef ARCH_UNTIL_FEATURESEXTRACTION_DFG_H
#define ARCH_UNTIL_FEATURESEXTRACTION_DFG_H

namespace boost{

    struct mnemonic_t{
        typedef vertex_property_tag kind;
    };

    struct op_str_t{
        typedef vertex_property_tag kind;
    };

    struct address_hex_str_t{
        typedef vertex_property_tag kind;
    };

    struct ins_str_t{
        typedef vertex_property_tag kind;
    };

    struct reg_used_t{
        typedef edge_property_tag kind;
    };

//    struct md5_t{
//        typedef graph_property_tag kind;
//    };

    typedef property<mnemonic_t, std::string, property<op_str_t, std::string,
            property<address_hex_str_t, std::string, property<ins_str_t, std::string>>>> VertexProperties;

    typedef property<reg_used_t, std::string> EdgeProperties;

//    typedef property<md5_t, std::string> GraphProperties;

//    typedef adjacency_list<listS, listS, bidirectionalS, VertexProperties> DFGraph;
//    typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties, GraphProperties> DFGraph;
    typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> DFGraph;

    typedef graph_traits<DFGraph> Traits;
    typedef Traits::vertex_descriptor  DFGVertex;
    typedef Traits::vertex_iterator DFGVertexIterator;
    typedef Traits::edge_descriptor DFGEdge;
    typedef property_map<DFGraph, mnemonic_t>::type DFGVMnemonicMap;
    typedef property_map<DFGraph, op_str_t>::type DFGVOpstrMap;
    typedef property_map<DFGraph, address_hex_str_t>::type DFGVAddressMap;
    typedef property_map<DFGraph, ins_str_t>::type DFGVInststrMap;
    typedef property_map<DFGraph, reg_used_t>::type DFGVRegUsedMap;
//    typedef property_map<DFGraph, md5_t>::type DFGMd5;

}


class DFG {

private:
    std::shared_ptr<boost::DFGraph> graph;



public:
    std::shared_ptr<boost::DFGraph> getGraph() const;


};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_DFG_H
