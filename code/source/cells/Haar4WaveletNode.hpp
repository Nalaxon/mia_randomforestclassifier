#ifndef HAAR4WAVELETNODE_HPP
#define HAAR4WAVELETNODE_HPP


#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class Haar4WaveletNode : public BaseNode {
  // Attributes
public:
  Haar4WaveletNode(bool is_vertical, double threshold, std::ostream* log_stream) : 
	  m_threshold(threshold), m_is_vertical(is_vertical), m_log_stream(log_stream) {}

private:
  double m_threshold;
  bool m_is_vertical;
  std::ostream* m_log_stream;

protected:
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
};


#endif