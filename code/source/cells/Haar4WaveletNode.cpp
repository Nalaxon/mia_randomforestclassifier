#include "Haar4WaveletNode.hpp"

#include "../tools/ImageTools.hpp"

Haar4WaveletNode::Direction Haar4WaveletNode::split(const cv::Mat& data) const
{
  int max_row_idx = data.rows - 1;
  int max_col_idx = data.cols - 1;
  int mean_row_idx = data.rows / 2;
  int mean_col_idx = data.cols / 2;

  double sum = ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, max_col_idx);

  double diff = 0;
  //|x-|
  //|-x|
  if (m_is_vertical)
  {
	  diff = ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, mean_col_idx);
	  diff =+ sum
			- ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, max_col_idx)
			- ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, mean_col_idx)
			+ diff;
  }
  //|-x|
  //|x-|
  else
  {
    diff = ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, max_col_idx)
		- ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, mean_col_idx);
	diff += ImageTools::get_pixel<float, 4, 2>(data, max_row_idx, mean_col_idx)
		- ImageTools::get_pixel<float, 4, 2>(data, mean_row_idx, mean_col_idx);;
  }

  diff = (sum - 2*diff);

  if (diff > m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}