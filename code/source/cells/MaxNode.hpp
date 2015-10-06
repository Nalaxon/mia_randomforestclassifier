#ifndef	MAXNODE_HPP
#define	MAXNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class MaxNode : public BaseNode
{
public:
	MaxNode(double threshold, std::ostream* log_stream)
		:m_threshold(threshold), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

private:
  
  float m_threshold;
  std::ostream* m_log_stream;
  

} ;

#endif	/* MAXNODE_HPP */

