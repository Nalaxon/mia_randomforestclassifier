#include "TwoPixelNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelNode::Direction TwoPixelNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    const auto& pixel1 = ImageTools::get_pixel<float, 4, 0>(data, m_y_1, m_x_1);
    const auto& pixel2 = ImageTools::get_pixel<float, 4, 0>(data, m_y_2, m_x_2);

	if (m_log_stream != nullptr)
		*m_log_stream << "TwoPixelNode;" << (pixel1 - pixel2) << ";" << std::endl;

    if ((pixel1 - pixel2) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}