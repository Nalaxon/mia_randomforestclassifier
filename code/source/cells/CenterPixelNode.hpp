#ifndef CENTERPIXELNODE_HPP
#define CENTERPIXELNODE_HPP

#include "Node.hpp"
#include "Label.hpp"

#include <opencv\cv.h>


/// class CenterPixelNode - 
class CenterPixelNode : public Node<Label, cv::Mat> {
  // Attributes
private:
  int m_x;
  int m_y;
};

#endif