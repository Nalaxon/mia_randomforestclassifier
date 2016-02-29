#include "BilateralNode.hpp"
#include "tools/ImageTools.hpp"

BilateralNode::Direction BilateralNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float BilateralNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat filtered, mat = cv::Mat(data[0], roi);
    cv::bilateralFilter(mat, filtered, 5, m_sigmaColor, m_sigmaSpace);

    if (m_log_stream != nullptr)
        *m_log_stream << "BilateralNode;" << m_threshold << ";" << filtered.at<float>(m_pixel) << "; " << std::endl;

    return filtered.at<float>(m_pixel);
}

void BilateralNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}
