#ifndef HAARWAVELETNODE_HPP
#define HAARWAVELETNODE_HPP


#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class HaarWaveletNode : public BaseNode {
  // Attributes
public:
  HaarWaveletNode(bool is_vertical, double threshold, std::ostream* log_stream) : 
	  m_threshold(threshold), m_is_vertical(is_vertical), m_log_stream(log_stream) {}

  virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  double m_threshold;
  bool m_is_vertical;
  std::ostream* m_log_stream;

protected:
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
};


#endif