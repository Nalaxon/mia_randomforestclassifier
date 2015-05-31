#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include "RTParameter.hpp"
#include "Node.hpp"

/// class RandomTree - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree {
  // Attributes
protected:
  RTParameter m_params;
  Node m_root;
  // Operations
public:
  RandomTree(RTParameter params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory);
  train(std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &samples);
  predict (DATA_TYPE &data, LABEL_TYPE &label);
};

#endif