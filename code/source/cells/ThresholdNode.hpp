#ifndef THRESHOLDNODE_HPP
#define THRESHOLDNODE_HPP

#include "BaseNode.hpp"
#include "Label.hpp"

#include <opencv2/opencv.hpp>

/// class ThresholdNode - 
class ThresholdNode : public BaseNode {
  // Attributes
private:
  size_t m_threshold;
};


#endif