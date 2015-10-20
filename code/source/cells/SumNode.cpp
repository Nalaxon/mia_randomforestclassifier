#include "SumNode.hpp"
#include "tools/ImageTools.hpp"

SumNode::Direction SumNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
  if (calc_thresh(data, roi) < m_threshold) {
    return Node::Direction::LEFT;
  }
  else {
    return Node::Direction::RIGHT;
  }
}

float SumNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[1], roi);
    int max_row_idx = mat.rows - 1;
    int max_col_idx = mat.cols - 1;

    double sum = ImageTools::sum_from_integral<float>(mat, cv::Rect(0, 0, max_row_idx, max_col_idx));

    if (m_log_stream != nullptr)
        *m_log_stream << "SumNode;" << sum << ";" << std::endl;

    return sum;
}

void SumNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}