#ifndef TWOREGIONSNODE_HPP
#define	TWOREGIONSNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class TwoRegionsNode : public BaseNode
{
public:
	TwoRegionsNode(const cv::Rect& region1, const cv::Rect& region2, float threshold, std::ostream* log_stream = nullptr)
		:m_region1(region1), m_region2(region2), m_threshold(threshold), m_log_stream(log_stream){}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
private:
  
  cv::Rect m_region1, m_region2;
  float m_threshold;
  std::ostream* m_log_stream;
} ;

#endif	/* TWOREGIONSNODE_HPP */

