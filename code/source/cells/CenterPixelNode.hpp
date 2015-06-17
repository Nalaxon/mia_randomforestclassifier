#ifndef CENTERPIXELNODE_HPP
#define CENTERPIXELNODE_HPP

#include "BaseNode.hpp"
#include "Label.hpp"

#include <opencv2/opencv.hpp>


class CenterPixelNode : public BaseNode
{
  
public:
  CenterPixelNode(int x, int y, float threshold) : m_x(x), m_y(y), m_threshold(threshold)
  {
  }
  
private:
  int m_x;
  int m_y;
  float m_threshold;

protected:
  
  virtual Direction split(const cv::Mat& data) const;
} ;


#endif