#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"

#include <memory>

template<typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory
{
public:

  virtual std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>>
          create(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples,
                 std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_left,
                 std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_right) = 0;

} ;

#endif
