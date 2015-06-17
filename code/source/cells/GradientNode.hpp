#ifndef GRADIENTNODE_HPP
#define GRADIENTNODE_HPP


#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class GradientNode : public BaseNode {
  // Attributes
public:
  GradientNode(double threshold) : m_threshold(threshold)
  {
  }

private:
  double m_threshold;

protected:
  virtual Direction split(const cv::Mat& data) const;
};


#endif