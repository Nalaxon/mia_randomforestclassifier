#ifndef HAAR4WAVELETNODE_HPP
#define HAAR4WAVELETNODE_HPP


#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class Haar4WaveletNode : public BaseNode {
  // Attributes
public:
  Haar4WaveletNode(bool is_vertical, double threshold) : 
    m_threshold(threshold), m_is_vertical(is_vertical)
  {
  }

private:
  double m_threshold;
  bool m_is_vertical;

protected:
  virtual Direction split(const cv::Mat& data) const;
};


#endif