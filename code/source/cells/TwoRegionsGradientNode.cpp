#include "TwoRegionsGradientNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsGradientNode::Direction TwoRegionsGradientNode::split(const cv::Mat& mat) const
{
	std::vector<cv::Mat> mat1(4), mat2(4);
	
	cv::split(mat(m_region1), mat1);
	cv::split(mat(m_region2), mat2);
	float mean1 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region1);
	float mean2 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region2);
	
	*m_log_stream << "TwoRegionsGradientNode;" << (mean1 - mean2) << ";" << std::endl;

	if ((mean1 - mean2) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


