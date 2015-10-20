#ifndef	HOGNODE_HPP
#define	HOGNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class HoGNode : public BaseNode
{
public:
	HoGNode(double threshold, std::ostream* log_stream)
		:m_threshold(threshold), m_log_stream(log_stream) {}

  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
  float m_threshold;
  std::ostream* m_log_stream;

} ;

#endif	/* HOGNODE_HPP */

