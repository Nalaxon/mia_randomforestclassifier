#ifndef RFPARAMETER_HPP
#define RFPARAMETER_HPP

#include "RTParameter.hpp"
#include "Histogram.hpp"

#include <functional>
#include <vector>

/// class RFParameter - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class RFParameter {
  // Associations
  RTParameter* m_treeParams;
  // Attributes
public:
  bool m_bagging;
private:
  RTParameter m_treeParams;
  std::function<double(std::vector<Histogram<LABEL_TYPE, DATA_TYPE)> m_ensemble_fct;
};

#endif
