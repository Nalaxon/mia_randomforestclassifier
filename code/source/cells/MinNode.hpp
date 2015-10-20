#ifndef	MINNODE_HPP
#define	MINNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class MinNode : public BaseNode
{
public:
	MinNode(double threshold, std::ostream* log_stream)
		:m_threshold(threshold), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
  float m_threshold;
  std::ostream* m_log_stream;
  

} ;

#endif	/* MINNODE_HPP */

