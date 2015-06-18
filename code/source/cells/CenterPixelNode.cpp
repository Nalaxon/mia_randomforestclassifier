#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const cv::Mat& data) const
{
    const auto& center_pixel = ImageTools::get_center_pixel<float, 3, 0>(data);
    const auto& other_pixel = ImageTools::get_pixel<float, 3, 0>(data, m_y, m_x);
    if ((center_pixel - other_pixel) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}
