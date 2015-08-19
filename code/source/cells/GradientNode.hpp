#ifndef GRADIENTNODE_HPP
#define GRADIENTNODE_HPP


#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv/cv.h>

/// class GradientNode - 
class GradientNode : public BaseNode {
  // Attributes
public:
	GradientNode(double threshold, std::ostream* log_stream = nullptr)
		: m_threshold(threshold), m_log_stream(log_stream)
  {
  }

private:
  double m_threshold;
  std::ostream* m_log_stream;

protected:
  virtual Direction split(const cv::Mat& data) const;
};


#endif