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

    struct line_number_t{
        typedef vertex_property_tag kind;
    };

    struct ins_str_t{
        typedef vertex_property_tag kind;
    };

    typedef property<mnemonic_t, std::string, property<op_str_t, std::string,
            property<address_hex_str_t, std::string>>> VertexProperties;

    typedef adjacency_list<listS, listS, bidirectionalS, VertexProperties> DFGraph;

}


class DFG {

private:
    std::shared_ptr<boost::DFGraph> graph;


public:
    std::shared_ptr<boost::DFGraph> getGraph() const;

};


#endif //ARCH_UNTIL_FEATURESEXTRACTION_DFG_H