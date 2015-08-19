#ifndef TWOPIXELGRADIENTNODE_HPP
#define TWOPIXELGRADIENTNODE_HPP

#include "BaseNode.hpp"
#include "CellLabel.hpp"

#include <vector>

#include <opencv2/opencv.hpp>

class TwoPixelGradientNode : public BaseNode
{
public:

  TwoPixelGradientNode(std::vector<unsigned int> x1, std::vector<unsigned int> y1,
	  std::vector<unsigned int> x2, std::vector<unsigned int> y2,
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

  virtual Direction split(const cv::Mat& data) const;

private:
  std::vector<unsigned int> m_x_1;
  std::vector<unsigned int> m_y_1;
  std::vector<unsigned int> m_x_2;
  std::vector<unsigned int> m_y_2;
  float m_threshold;
  std::ostream* m_log_stream;
} ;


#endif