#ifndef THRESHOLDNODE_HPP
#define THRESHOLDNODE_HPP

#include "Node.hpp"
#include "Label.hpp"

#include <opencv2\opencv.hpp>

/// class ThresholdNode - 
class ThresholdNode : public Node<Label, cv::Mat> {
  // Attributes
private:
  size_t m_threshold;
};


#endif