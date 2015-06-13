#include "TwoPixelNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelNode::Direction TwoPixelNode::split(const cv::Mat& data) const
{
    const auto& pixel1 = ImageTools::get_pixel<float, 3, 0>(data, m_y_1, m_x_1);
    const auto& pixel2 = ImageTools::get_pixel<float, 3, 0>(data, m_y_2, m_x_2);
    if (pixel1 < pixel2) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}