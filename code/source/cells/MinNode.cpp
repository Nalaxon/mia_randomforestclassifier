#include "MinNode.hpp"
#include "Tools/ImageTools.hpp"

MinNode::Direction MinNode::split(const cv::Mat& mat) const
{
	std::vector<cv::Mat> channels = ImageTools::extract_channels<4>(mat);
	double val;
	cv::minMaxLoc(channels[3], &val);
	
	if (m_log_stream != nullptr)
		*m_log_stream << "MinNode;" << m_threshold << ";" << val << "; " << std::endl;

	if (val < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


