#ifndef TWOPIXELNODE_HPP
#define TWOPIXELNODE_HPP

#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <opencv2/opencv.hpp>

class TwoPixelNode : public BaseNode
{
public:

	TwoPixelNode(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
		float threshold, std::ostream* log_stream = nullptr)
  : m_x_1(x1),
  m_y_1(y1),
  m_x_2(x2),
  m_y_2(y2),
  m_threshold(threshold),
  m_log_stream(log_stream)
  {
  }

protected:

	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

  unsigned int m_x_1;
  unsigned int m_y_1;
  unsigned int m_x_2;
  unsigned int m_y_2;
  float m_threshold;
  std::ostream* m_log_stream;
} ;


#endif