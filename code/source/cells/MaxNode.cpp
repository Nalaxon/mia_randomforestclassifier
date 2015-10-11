#include "MaxNode.hpp"
#include "Tools/ImageTools.hpp"

MaxNode::Direction MaxNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[2], roi);
	double val, tmp;
	cv::minMaxLoc(mat, &tmp, &val);
	
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


