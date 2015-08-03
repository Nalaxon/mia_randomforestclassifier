#ifndef	CANNYEDGEMOMENT00NODE_HPP
#define	CANNYEDGEMOMENT00NODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class CannyEdgeMoment00Node : public BaseNode
{
public:
	CannyEdgeMoment00Node(double canny_threshold, double threshold, std::ostream* log_stream)
		:m_canny_threshold(canny_threshold), m_threshold(threshold), m_log_stream(log_stream) {}
  
  virtual Direction split(const cv::Mat& mat) const;
  
private:
  
  float m_threshold, m_canny_threshold;
  std::ostream* m_log_stream;
  

} ;

#endif	/* CANNYEDGEMOMENT00NODE_HPP */

