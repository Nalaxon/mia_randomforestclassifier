#ifndef LEAFNODE_HPP
#define LEAFNODE_HPP

#include "Histogram.hpp"
#include "types.hpp"
#include "memory.hpp"
#include "Node.hpp"

#include <iostream>
#include <sstream>

template <typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class LeafNode : public Node<LABEL_TYPE, DATA_TYPE, ROI_TYPE>
{
public:
  
  using SuperType = Node<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  LeafNode()
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");
  }

protected:
  
  //----------------------------------------------------------------------------
  virtual typename SuperType::Direction split(const DATA_TYPE& data, const ROI_TYPE& roi) const
  {
    return SuperType::Direction::NONE;
  }

  virtual void setThreshold(const DATA_TYPE& data, const ROI_TYPE& roi) {};

public:

} ;

#endif
