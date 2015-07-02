#include "TwoRegionsGradientNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsGradientNode::TwoRegionsGradientNode(const cv::Rect& region1, const cv::Rect& region2, float threshold)
:m_region1(region1), m_region2(region2), m_threshold(threshold)
{    
}

TwoRegionsGradientNode::Direction TwoRegionsGradientNode::split(const cv::Mat& mat) const
{
	std::vector<cv::Mat> mat1, mat2;
	std::cout << "here we go" << std::endl;
	cv::split(mat(m_region1), mat1);
	cv::split(mat(m_region2), mat2);
	std::cout << "after we go" << std::endl;
	cv::Scalar mean = cv::mean(mat1[3] - mat2[3]);
	if (mean.val[0] < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


