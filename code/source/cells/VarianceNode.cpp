#include "VarianceNode.hpp"
#include "Tools/ImageTools.hpp"

VarianceNode::Direction VarianceNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	cv::Mat psd;
	cv::dft(data[2].mul(data[2]), psd);
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


