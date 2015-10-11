#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat mat = cv::Mat(data[0], roi);
    const auto& center_pixel = ImageTools::get_center_pixel<float>(mat);
    const auto& other_pixel = ImageTools::get_pixel<float>(mat, m_y, m_x);

	if (m_log_stream != nullptr)
		*m_log_stream << "CenterPixelNode;" << (center_pixel - other_pixel) << ";" << std::endl;

    if ((center_pixel - other_pixel) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}
