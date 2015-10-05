#include "BilateralNode.hpp"
#include "Tools/ImageTools.hpp"

BilateralNode::Direction BilateralNode::split(const std::vector<cv::Mat>& mat, const cv::Rect& roi) const
{
	std::vector<cv::Mat> channels = ImageTools::extract_channels<4>(mat);
	cv::Mat filtered;
	cv::bilateralFilter(channels[0], filtered, 5, m_sigmaColor, m_sigmaSpace);
	
	if (m_log_stream != nullptr)
		*m_log_stream << "BilateralNode;" << m_threshold << ";" << filtered.at<float>(m_pixel) << "; " << std::endl;

	if (filtered.at<float>(m_pixel) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


