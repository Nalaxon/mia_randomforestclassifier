#include "CenterPixelNode.hpp"

#include "../tools/ImageTools.hpp"

CenterPixelNode::Direction CenterPixelNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    

    if (calc_thresh(data, roi) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}

float CenterPixelNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat mat = cv::Mat(data[0], roi);
    const auto& center_pixel = ImageTools::get_center_pixel<float>(mat);
    const auto& other_pixel = ImageTools::get_pixel<float>(mat, m_y, m_x);

    if (m_log_stream != nullptr)
        *m_log_stream << "CenterPixelNode;" << (center_pixel - other_pixel) << ";" << std::endl;

    return (center_pixel - other_pixel);
}

void CenterPixelNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}
