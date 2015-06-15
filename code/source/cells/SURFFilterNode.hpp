#ifndef SURFFILTERNODE_HPP
#define SURFFILTERNODE_HPP


#include "Node.hpp"
#include "Label.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class SURFFilterNode : public Node<Label, cv::Mat> {
  // Attributes
public:
  SURFFilterNode(bool is_vertical, double threshold) :
    m_is_vertical(is_vertical), m_threshold(threshold)
  {
  }

private:
  double m_threshold;
  bool m_is_vertical;

protected:
  virtual Direction split(const cv::Mat& data) const;
};


#endif