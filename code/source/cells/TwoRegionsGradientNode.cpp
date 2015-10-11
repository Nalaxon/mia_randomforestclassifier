#include "TwoRegionsGradientNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsGradientNode::Direction TwoRegionsGradientNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[1], roi);
	std::vector<cv::Mat> mat1(4), mat2(4);

	float mean1 = ImageTools::mean_from_integral<float>(mat, m_region1);
	float mean2 = ImageTools::mean_from_integral<float>(mat, m_region2);
	double sub = abs(mean1 - mean2);
	
	if (m_log_stream != nullptr)
		*m_log_stream << "TwoRegionsGradientNode;" << sub << std::endl;

	if (sub < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


