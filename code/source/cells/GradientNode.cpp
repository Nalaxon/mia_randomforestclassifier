#include "GradientNode.hpp"

#include "../tools/ImageTools.hpp"

GradientNode::Direction GradientNode::split(const cv::Mat& data) const
{
  int max_row_idx = data.rows - 1;
  int max_col_idx = data.cols - 1;
  double sum = ImageTools::get_pixel<float, 4, 1>(data, max_row_idx, max_col_idx) 
    - ImageTools::get_pixel<float, 4, 1>(data, 0, max_col_idx)
    - ImageTools::get_pixel<float, 4, 1>(data, max_row_idx, 0)
    + ImageTools::get_pixel<float, 4, 1>(data, 0, 0);

  *m_log_stream << "GradientNode;" << sum << ";" << std::endl;

  if (sum > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}