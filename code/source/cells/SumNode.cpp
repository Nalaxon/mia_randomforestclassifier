#include "SumNode.hpp"
#include "tools/ImageTools.hpp"

SumNode::Direction SumNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
  int max_row_idx = data[0].rows - 1;
  int max_col_idx = data[0].cols - 1;

  double top_left = ImageTools::get_pixel<float, 4, 2>(data, 0, 0);
  double top_right = ImageTools::get_pixel<float, 4, 2>(data, 0, max_col_idx);
  double bottom_left = ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, 0);
  double bottom_right = ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, max_col_idx);

  double sum = bottom_right - top_right - bottom_left + top_left;

  if (m_log_stream != nullptr)
	  *m_log_stream << "SumNode;" << sum << ";" << std::endl;

  if (sum > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}
