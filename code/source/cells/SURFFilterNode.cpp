#include "SURFFilterNode.hpp"

#include "../tools/ImageTools.hpp"

SURFFilterNode::Direction SURFFilterNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[1], roi);
  int max_row_idx = data[1].rows - 1;
  int max_col_idx = data[1].cols - 1;
  int fst_row_idx = data[1].rows / 3;
  int fst_col_idx = data[1].cols / 3;
  int snd_row_idx = 2 * data[1].rows / 3;
  int snd_col_idx = 2 * data[1].cols / 3;

  double sum = ImageTools::sum_from_integral<float>(mat,
      cv::Rect(0, 0, max_col_idx, max_row_idx));

  double middle_part = 0;
  if (m_is_vertical)
      middle_part = ImageTools::sum_from_integral<float>(mat,
          cv::Rect(0, fst_row_idx, max_col_idx, snd_row_idx - fst_row_idx));
  else
      middle_part = ImageTools::sum_from_integral<float>(mat,
      cv::Rect(fst_col_idx, 0, snd_col_idx - fst_row_idx, max_row_idx));


  double diff = (sum - 2 * middle_part);

  if (m_log_stream != nullptr)
	  *m_log_stream << "SURFFilterNode;" << diff << ";" << std::endl;

  if (diff > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}