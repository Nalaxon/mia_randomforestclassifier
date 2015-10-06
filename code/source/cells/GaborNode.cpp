#include "GaborNode.hpp"
#include "Tools/ImageTools.hpp"

GaborNode::Direction GaborNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	int kernel_size = data[0].cols / 3;
	cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), m_sig, m_th, m_lm, m_gm, m_ps);

	cv::Mat dest;
	cv::filter2D(data[0], dest, CV_32F, kernel);

	double mean = std::abs(cv::mean(dest).val[0]);

	if (m_log_stream != nullptr)
		*m_log_stream << "GaborNode;" << mean << ";" << std::endl;

	if (mean < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


