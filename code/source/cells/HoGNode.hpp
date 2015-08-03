#ifndef	HOGNODE_HPP
#define	HOGNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class HoGNode : public BaseNode
{
public:
	HoGNode(double threshold, std::ostream* log_stream)
		:m_threshold(threshold), m_log_stream(log_stream) {}

  
  virtual Direction split(const cv::Mat& mat) const;
  
private:
  
  float m_threshold;
  std::ostream* m_log_stream;

} ;

#endif	/* HOGNODE_HPP */

