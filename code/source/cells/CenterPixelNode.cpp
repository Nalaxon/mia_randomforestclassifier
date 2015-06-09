#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const cv::Mat& data) const
{
    float center_pixel = ImageTools::getCenterPixel<float>(data);
    float other_pixel = data.at<float>(m_y, m_x);
    if (center_pixel < other_pixel) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}
