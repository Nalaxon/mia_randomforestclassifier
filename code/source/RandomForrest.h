#include "RFParams.h"

/// class RandomForrest - 
template <typename T>
class RandomForrest {
  // Attributes
protected:
  RFParams m_params;
  size_t m_nClasses;
  // Operations
public:
  RandomForrest (RFParams params);
  train (std::vector<T> data, std::vector<int> labels);
  predict (std::vector<T> data, std::vector<int> labels);
};

