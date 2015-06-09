#ifndef GRADIENTNODE_HPP
#define GRADIENTNODE_HPP


#include "Node.hpp"
#include "Label.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class GradientNode : public Node<Label, cv::Mat> {
  // Attributes
private:
  size_t m_threshold;
};


#endif