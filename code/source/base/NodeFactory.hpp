#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"

#include <memory>

/// class NodeFactory - 
template<typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory
{
    // Operations
public:
    void create(std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>>& node,
                const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples,
                std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_left,
                std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_right);
};

#endif
