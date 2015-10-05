#include "MaxNode.hpp"
#include "Tools/ImageTools.hpp"

MaxNode::Direction MaxNode::split(const std::vector<cv::Mat>& mat, const cv::Rect& roi) const
{
	std::vector<cv::Mat> channels = ImageTools::extract_channels<4>(mat);
	double val, tmp;
	cv::minMaxLoc(channels[3], &tmp, &val);
	
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


