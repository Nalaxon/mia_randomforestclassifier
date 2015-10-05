#ifndef TWOREGIONSGRADIENTNODE_HPP
#define	TWOREGIONSGRADIENTNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class TwoRegionsGradientNode : public BaseNode
{
public:
	TwoRegionsGradientNode(const cv::Rect& region1, const cv::Rect& region2, float threshold, std::ostream* log_stream = nullptr)
		:m_region1(region1), m_region2(region2), m_threshold(threshold), m_log_stream(log_stream){}
  
	virtual Direction split(const std::vector<cv::Mat>& mat, const cv::Rect& roi) const;
  virtual std::string get_ClassName() { return (typeid(*this)).name(); };
  
private:
  
  cv::Rect m_region1, m_region2;
  float m_threshold;
  std::ostream* m_log_stream;
} ;

#endif	/* TWOREGIONSGRADIENTNODE_HPP */

