#include "TwoPixelNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelNode::Direction TwoPixelNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    if (calc_thresh(data, roi) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}

float TwoPixelNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[0], roi);
    const auto& pixel1 = mat.at<float>(m_y_1, m_x_1);
    const auto& pixel2 = mat.at<float>(m_y_2, m_x_2);

    if (m_log_stream != nullptr)
        *m_log_stream << "TwoPixelNode; " << m_threshold << "; " << (pixel1 - pixel2) << "; " << std::endl;

    return (pixel1 - pixel2);
}

void TwoPixelNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}