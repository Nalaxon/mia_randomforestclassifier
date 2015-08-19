#include "TwoPixelNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelNode::Direction TwoPixelNode::split(const cv::Mat& data) const
{
    const auto& pixel1 = ImageTools::get_pixel<float, 4, 0>(data, m_y_1, m_x_1);
    const auto& pixel2 = ImageTools::get_pixel<float, 4, 0>(data, m_y_2, m_x_2);

	*m_log_stream << "TwoPixelNode;" << (pixel1 - pixel2) << ";" << std::endl;

    if ((pixel1 - pixel2) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}