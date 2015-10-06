#include "MinNode.hpp"
#include "Tools/ImageTools.hpp"

MinNode::Direction MinNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	double val;
	cv::minMaxLoc(data[2], &val);
	
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


