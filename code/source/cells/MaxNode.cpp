#include "MaxNode.hpp"
#include "tools/ImageTools.hpp"

MaxNode::Direction MaxNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float MaxNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[2], roi);
    double val, tmp;
    cv::minMaxLoc(mat, &tmp, &val);

    if (m_log_stream != nullptr)
        *m_log_stream << "MaxNode;" << m_threshold << ";" << val << "; " << std::endl;

    return val;
}

void MaxNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}
