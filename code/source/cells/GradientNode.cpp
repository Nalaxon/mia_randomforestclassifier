#include "GradientNode.hpp"

#include "../tools/ImageTools.hpp"

GradientNode::Direction GradientNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
  int max_row_idx = data[5].rows - 1;
  int max_col_idx = data[5].cols - 1;
  double sum = ImageTools::sum_from_integral<float>(data[5], cv::Rect(0, 0, max_row_idx, max_col_idx));

  if (m_log_stream != nullptr)
	  *m_log_stream << "GradientNode;" << sum << ";" << std::endl;

  if (sum > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}