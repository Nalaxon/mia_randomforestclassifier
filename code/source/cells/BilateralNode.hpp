#ifndef	BILATERALNODE_HPP
#define	BILATERALNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class BilateralNode : public BaseNode
{
public:
	BilateralNode(float sigmaColor, float sigmaSpace, cv::Point2f &pixel, float threshold, std::ostream* log_stream)
		:m_sigmaColor(sigmaColor), m_sigmaSpace(sigmaSpace), m_pixel(pixel),
		m_threshold(threshold), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
private:
  
  float m_threshold, m_sigmaColor, m_sigmaSpace;
  cv::Point2f m_pixel;
  std::ostream* m_log_stream;
  

} ;

#endif	/* BILATERALNODE_HPP */

