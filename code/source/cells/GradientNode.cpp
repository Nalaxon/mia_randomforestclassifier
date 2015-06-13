#include "GradientNode.hpp"

#include "../tools/ImageTools.hpp"

GradientNode::Direction GradientNode::split(const cv::Mat& data) const
{
  cv::Mat value_channel = ImageTools::extractChannel<3, 1>(data);
  int max_row_idx = value_channel.rows - 1;
  int max_col_idx = value_channel.cols - 1;
  double sum = value_channel.at<float>(max_row_idx, max_col_idx) 
    - value_channel.at<float>(0, max_col_idx) 
    - value_channel.at<float>(max_row_idx, 0) 
    + value_channel.at<float>(0, 0);
  if (sum > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}