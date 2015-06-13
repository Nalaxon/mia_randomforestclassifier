#include "TwoPixelNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelNode::Direction TwoPixelNode::split(const cv::Mat& data) const
{
    cv::Mat value_channel = ImageTools::extractChannel<3, 0>(data);
    const auto& pixel1 = value_channel.at<float>(m_y_1, m_x_1);
    const auto& pixel2 = value_channel.at<float>(m_y_2, m_x_2);
    if (pixel1 < pixel2) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}