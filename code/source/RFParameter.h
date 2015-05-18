#include "RTParameter.h"

/// class RFParameter - 
class RFParameter {
  // Associations
  RTParameter* m_treeParams;
  // Attributes
public:
  std::function<double(std::vector<double>)> m_ensemble_fct;
protected:
  RTParameter m_treeParams;
};

