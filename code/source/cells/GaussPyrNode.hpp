#ifndef	GAUSSPYRENODE_HPP
#define	GAUSSPYRNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class GaussPyrNode : public BaseNode
{
public:
	GaussPyrNode(int level, int test, double threshold, std::ostream* log_stream)
		:m_level(level), m_test(test), m_threshold(threshold),
		m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
private:
  
  float m_threshold;
  int m_level, m_test;
  std::ostream* m_log_stream;
  

} ;

#endif	/* GAUSSPYRNODE_HPP */

