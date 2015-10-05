#ifndef	SDNODE_HPP
#define	SDNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class SDNode : public BaseNode
{
public:
	SDNode(double threshold, std::ostream* log_stream)
		:m_threshold(threshold), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& mat, const cv::Rect& roi) const;
  
private:
  
  float m_threshold;
  std::ostream* m_log_stream;
  

} ;

#endif	/* SDNODE_HPP */

