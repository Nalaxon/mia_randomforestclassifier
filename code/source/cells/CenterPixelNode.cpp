#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const cv::Mat& data) const
{
    const auto& center_pixel = ImageTools::get_center_pixel<float, 4, 0>(data);
    const auto& other_pixel = ImageTools::get_pixel<float, 4, 0>(data, m_y, m_x);

	if (m_log_stream != nullptr)
		*m_log_stream << "CenterPixelNode;" << (center_pixel - other_pixel) << ";" << std::endl;

    if ((center_pixel - other_pixel) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}
