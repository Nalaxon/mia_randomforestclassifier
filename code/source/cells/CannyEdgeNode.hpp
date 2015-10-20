#ifndef	CANNYEDGENODE_HPP
#define	CANNYEDGENODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class CannyEdgeNode : public BaseNode
{
public:
	CannyEdgeNode(double canny_threshold, double threshold, std::ostream* log_stream)
		:m_canny_threshold(canny_threshold), m_threshold(threshold), m_log_stream(log_stream) {}

  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);
  
private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
  float m_threshold, m_canny_threshold;
  std::ostream* m_log_stream;
} ;

#endif	/* CANNYEDGENODE_HPP */

