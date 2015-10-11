#include "SDNode.hpp"
#include "Tools/ImageTools.hpp"

SDNode::Direction SDNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat mean, stdev, mat = cv::Mat(data[2], roi);

	cv::meanStdDev(mat, mean, stdev);
	
	if (m_log_stream != nullptr)
		*m_log_stream << "SDNode;" << m_threshold << ";" << stdev.at<double>(0) << "; " << std::endl;

	if (stdev.at<double>(0) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


