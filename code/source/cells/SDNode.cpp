#include "SDNode.hpp"
#include "Tools/ImageTools.hpp"

SDNode::Direction SDNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float SDNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat mean, stdev, mat = cv::Mat(data[2], roi);

    cv::meanStdDev(mat, mean, stdev);

    if (m_log_stream != nullptr)
        *m_log_stream << "SDNode;" << m_threshold << ";" << stdev.at<double>(0) << "; " << std::endl;

    return stdev.at<double>(0);
}

void SDNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}