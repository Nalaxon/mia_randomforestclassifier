#include "TwoPixelNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelNode::Direction TwoPixelNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[0], roi);
    const auto& pixel1 = mat.at<float>(m_y_1, m_x_1);
    const auto& pixel2 = mat.at<float>(m_y_2, m_x_2);

	if (m_log_stream != nullptr)
		*m_log_stream << "TwoPixelNode;" << (pixel1 - pixel2) << ";" << std::endl;

    if ((pixel1 - pixel2) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}