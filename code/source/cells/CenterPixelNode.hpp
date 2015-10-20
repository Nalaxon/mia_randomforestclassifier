#ifndef CENTERPIXELNODE_HPP
#define CENTERPIXELNODE_HPP

#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv2/opencv.hpp>


class CenterPixelNode : public BaseNode
{
  
public:
	CenterPixelNode(int x, int y, float threshold, std::ostream* log_stream = nullptr)
		: m_x(x), m_y(y), m_threshold(threshold), m_log_stream(log_stream)
  {
  }
  
    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  int m_x;
  int m_y;
  float m_threshold;
  std::ostream* m_log_stream;

protected:
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
} ;


#endif