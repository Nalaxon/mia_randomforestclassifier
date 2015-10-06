#include "MaxNode.hpp"
#include "Tools/ImageTools.hpp"

MaxNode::Direction MaxNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	double val, tmp;
	cv::minMaxLoc(data[2], &tmp, &val);
	
	if (m_log_stream != nullptr)
		*m_log_stream << "MaxNode;" << m_threshold << ";" << val << "; " << std::endl;

	if (val < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


