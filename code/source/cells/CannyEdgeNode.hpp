#ifndef	CANNYEDGENODE_HPP
#define	CANNYEDGENODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class CannyEdgeNode : public BaseNode
{
public:
	CannyEdgeNode(double canny_threshold, double threshold, std::ostream* log_stream)
		:m_canny_threshold(canny_threshold), m_threshold(threshold), m_log_stream(log_stream) {}

  
  virtual Direction split(const cv::Mat& mat) const;
  
private:
  
  float m_threshold, m_canny_threshold;
  std::ostream* m_log_stream;
} ;

#endif	/* CANNYEDGENODE_HPP */

