#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    const auto& center_pixel = ImageTools::get_center_pixel<float>(data[0]);
    const auto& other_pixel = ImageTools::get_pixel<float>(data[0], m_y, m_x);

	if (m_log_stream != nullptr)
		*m_log_stream << "CenterPixelNode;" << (center_pixel - other_pixel) << ";" << std::endl;

    if ((center_pixel - other_pixel) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}
