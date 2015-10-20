#include "MinNode.hpp"
#include "Tools/ImageTools.hpp"

MinNode::Direction MinNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float MinNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[2], roi);
    double val;
    cv::minMaxLoc(mat, &val);

    if (m_log_stream != nullptr)
        *m_log_stream << "MinNode;" << m_threshold << ";" << val << "; " << std::endl;

    return val;
}

void MinNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}