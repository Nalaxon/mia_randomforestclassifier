#include "TwoRegionsNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsNode::Direction TwoRegionsNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	if (calc_thresh(data, roi) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}

float TwoRegionsNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[1], roi);
    float mean1 = ImageTools::mean_from_integral<float>(mat, m_region1);
    float mean2 = ImageTools::mean_from_integral<float>(mat, m_region2);

    if (m_log_stream != nullptr)
        *m_log_stream << "TwoRegionsNode;" << (mean1 - mean2) << ";" << mean1 << ";" << mean2 << ";"
        << m_region1.x << ";" << m_region1.y << ";" << m_region1.width << ";" << m_region1.height << std::endl;

    return (mean1 - mean2);
}

void TwoRegionsNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}