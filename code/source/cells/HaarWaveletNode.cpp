#include "HaarWaveletNode.hpp"

#include "../tools/ImageTools.hpp"

HaarWaveletNode::Direction HaarWaveletNode::split(const cv::Mat& data) const
{
  int max_row_idx = data.rows - 1;
  int max_col_idx = data.cols - 1;
  int mean_row_idx = data.rows / 2;
  int mean_col_idx = data.cols / 2;

  double base = ImageTools::get_pixel<float, 4, 2>(data, 0, 0);
  double top_left = ImageTools::get_pixel<float, 4, 2>(data, 0, max_col_idx);
  double bottom_right = ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, 0);

  double sum = ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, max_col_idx)
    - top_left
    - bottom_right
    + base;

  double diff = 0;
  if (m_is_vertical)
  {
    diff = ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, max_col_idx)
      - top_left
      - ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, 0)
      + base;
  } 
  else
  {
    diff = ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, mean_col_idx)
      - bottom_right
      - ImageTools::get_pixel<float, 4, 2>(data, 0, mean_col_idx)
      + base;
  }

  diff = (sum - 2*diff);

  if (m_log_stream != nullptr)
	  *m_log_stream << "HaarWaveletNode;" << diff << ";" << std::endl;

  if (diff > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}