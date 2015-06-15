#ifndef RFPARAMETER_HPP
#define RFPARAMETER_HPP

#include "RTParameter.hpp"
#include "Histogram.hpp"
#include "types.hpp"

template<typename LABEL_TYPE, typename DATA_TYPE>
struct RFParameter
{ 
  // number of trees to use
  unsigned int m_num_trees;
  
  // is bagging enabled?
  bool m_bagging;
  
  // tree parameters
  RTParameter m_tree_params;

} ;

#endif
