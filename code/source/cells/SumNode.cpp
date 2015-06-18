#include "SumNode.hpp"
#include "tools/ImageTools.hpp"


SumNode::SumNode(double threshhold) : m_threshold(threshhold)
{
}

SumNode::Direction SumNode::split(const cv::Mat& data) const
{
  int max_row_idx = data.rows - 1;
  int max_col_idx = data.cols - 1;

  double top_left = ImageTools::get_pixel<float, 3, 2>(data, 0, 0);
  double top_right = ImageTools::get_pixel<float, 3, 2>(data, 0, max_col_idx);
  double bottom_left = ImageTools::get_pixel<float, 3, 2>(data, max_row_idx, 0);
  double bottom_right = ImageTools::get_pixel<float, 3, 2>(data, max_row_idx, max_col_idx);

  double sum = bottom_right - top_right - bottom_left + top_left;

  if (sum > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}
