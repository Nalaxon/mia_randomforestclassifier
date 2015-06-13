#ifndef TWOPIXELNODE_HPP
#define TWOPIXELNODE_HPP

#include "base/Node.hpp"
#include "Label.hpp"

#include <opencv2/opencv.hpp>

class TwoPixelNode : public Node<Label, cv::Mat>
{
public:

  TwoPixelNode(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
  : m_x_1(x1),
  m_y_1(y1),
  m_x_2(x2),
  m_y_2(y2)
  {
  }

protected:

  virtual Direction split(const cv::Mat& data) const;

private:
  unsigned int m_x_1;
  unsigned int m_y_1;
  unsigned int m_x_2;
  unsigned int m_y_2;
} ;


#endif