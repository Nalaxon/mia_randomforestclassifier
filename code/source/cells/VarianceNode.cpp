#include "VarianceNode.hpp"
#include "Tools/ImageTools.hpp"

VarianceNode::Direction VarianceNode::split(const std::vector<cv::Mat>& mat, const cv::Rect& roi) const
{
	std::vector<cv::Mat> channels = ImageTools::extract_channels<4>(mat);
	cv::Mat psd;
	cv::dft(channels[3].mul(channels[3]), psd);
	float var = cv::mean(psd).val[0];

	if (m_log_stream != nullptr)
		*m_log_stream << "VarianceNode;" << m_threshold << ";" << var << "; " << std::endl;

	if (var < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


