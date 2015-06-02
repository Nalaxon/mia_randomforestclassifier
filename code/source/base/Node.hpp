#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"

/// class Node - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class Node {
  // Attributes
private:
  Histogram m_histogram;
protected:
  double m_threshold;
  Node m_leftChild;
  Node m_rightChild;
  // Operations
public:
  predict (DATA_TYPE &data, LABEL_TYPE &label);
protected:
	virtual split(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S,
		std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S_left,
		std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S_right) = 0;
};

#endif
