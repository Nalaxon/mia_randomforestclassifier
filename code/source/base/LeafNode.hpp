#ifndef LEAFNODE_HPP
#define LEAFNODE_HPP

#include "Histogram.hpp"
#include "types.hpp"
#include "memory.hpp"

#include <iostream>
#include <sstream>

template <typename LABEL_TYPE, typename DATA_TYPE>
class LeafNode : public Node<LABEL_TYPE, DATA_TYPE>
{
public:

  LeafNode()
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");
  }

protected:
  
  //----------------------------------------------------------------------------
  virtual Direction split(const DATA_TYPE& data) const
  {
    return Direction::NONE;
  }

public:

} ;

#endif
