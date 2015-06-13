#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const cv::Mat& data) const
{
    cv::Mat value_channel = ImageTools::extractChannel<3, 1>(data);
    float center_pixel = ImageTools::getCenterPixel<float>(value_channel);
    float other_pixel = value_channel.at<float>(m_y, m_x);
    if (center_pixel < other_pixel) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}
