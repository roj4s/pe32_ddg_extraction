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

    typedef property<mnemonic_t, std::string, property<op_str_t, std::string,
            property<address_hex_str_t, std::string, property<ins_str_t, std::string>>>> VertexProperties;

    typedef adjacency_list<listS, listS, bidirectionalS, VertexProperties> DFGraph;

    typedef graph_traits<DFGraph> Traits;
    typedef Traits::vertex_descriptor  DFGVertex;
    typedef Traits::edge_descriptor DFGEdge;
    typedef property_map<DFGraph, mnemonic_t>::type DFGVMnemonicMap;
    typedef property_map<DFGraph, op_str_t>::type DFGVOpstrMap;
    typedef property_map<DFGraph, address_hex_str_t>::type DFGVAddressMap;
    typedef property_map<DFGraph, ins_str_t>::type DFGVInststrMap;

}


class DFG {

private:
    std::shared_ptr<boost::DFGraph> graph;



public:
    std::shared_ptr<boost::DFGraph> getGraph() const;


};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_DFG_H
