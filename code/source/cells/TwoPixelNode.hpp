#ifndef TWOPIXELNODE_HPP
#define TWOPIXELNODE_HPP

#include "Node.hpp"
#include "Label.hpp"

#include <opencv2/opencv.hpp>

/// class TwoPixelNode - 
class TwoPixelNode : public Node<Label, cv::Mat> {
  // Attributes
private:
  int m_x_1;
  int m_y_1;
  int m_x_2;
  int m_y_2;
};


#endif