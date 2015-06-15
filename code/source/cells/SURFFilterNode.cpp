#include "SURFFilterNode.hpp"

#include "../tools/ImageTools.hpp"

SURFFilterNode::Direction SURFFilterNode::split(const cv::Mat& data) const
{
  int max_row_idx = data.rows - 1;
  int max_col_idx = data.cols - 1;
  int fst_row_idx = data.rows / 3;
  int fst_col_idx = data.cols / 3;
  int snd_row_idx = 2*data.rows / 3;
  int snd_col_idx = 2*data.cols / 3;

  double sum = ImageTools::get_pixel<float, 3, 2>(data, max_row_idx, max_col_idx)
    - ImageTools::get_pixel<float, 3, 2>(data, 0, max_col_idx)
    - ImageTools::get_pixel<float, 3, 2>(data, max_row_idx, 0)
    + ImageTools::get_pixel<float, 3, 2>(data, 0, 0);;

  double middle_part = 0;
  if (m_is_vertical)
  {
    middle_part = ImageTools::get_pixel<float, 3, 2>(data, snd_row_idx, max_col_idx)
      - ImageTools::get_pixel<float, 3, 2>(data, fst_row_idx, max_col_idx)
      - ImageTools::get_pixel<float, 3, 2>(data, snd_row_idx, 0)
      + ImageTools::get_pixel<float, 3, 2>(data, fst_row_idx, 0);
  } 
  else
  {
    middle_part = ImageTools::get_pixel<float, 3, 2>(data, max_row_idx, snd_col_idx)
      - ImageTools::get_pixel<float, 3, 2>(data, 0, snd_col_idx)
      - ImageTools::get_pixel<float, 3, 2>(data, max_row_idx, fst_col_idx)
      + ImageTools::get_pixel<float, 3, 2>(data, 0, fst_col_idx);
  }

  double diff = (sum - 2 * middle_part);

  if (diff > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}