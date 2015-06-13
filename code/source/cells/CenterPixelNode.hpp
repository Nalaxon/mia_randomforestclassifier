#ifndef CENTERPIXELNODE_HPP
#define CENTERPIXELNODE_HPP

#include "base/Node.hpp"
#include "Label.hpp"

#include <opencv2/opencv.hpp>


class CenterPixelNode : public Node<Label, cv::Mat>
{
  
public:
  CenterPixelNode(int x, int y) : m_x(x), m_y(y)
  {
  }
  
private:
  int m_x;
  int m_y;

protected:
  
  virtual Direction split(const cv::Mat& data) const;
} ;


#endif