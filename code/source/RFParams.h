#include "RTParams.h"

/// class RFParams - 
class RFParams {
  // Attributes
public:
  std::function<double(std::vector<double>)> m_ensemble_fct;
protected:
  RTParams m_treeParams;
};

