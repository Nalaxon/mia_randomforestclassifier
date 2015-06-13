#ifndef GRADIENTNODE_HPP
#define GRADIENTNODE_HPP


#include "Node.hpp"
#include "Label.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class GradientNode : public Node<Label, cv::Mat> {
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