#include "SDNode.hpp"
#include "Tools/ImageTools.hpp"

SDNode::Direction SDNode::split(const cv::Mat& mat) const
{
	cv::Mat mean, stdev;

	cv::meanStdDev(mat, mean, stdev);
	
	if (m_log_stream != nullptr)
		*m_log_stream << "SDNode;" << m_threshold << ";" << stdev.at<double>(3) << "; " << std::endl;

	if (stdev.at<double>(3) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


