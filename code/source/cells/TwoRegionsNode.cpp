#include "TwoRegionsNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsNode::Direction TwoRegionsNode::split(const cv::Mat& mat) const
{
    float mean1 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region1);
    float mean2 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region2);
    
	*m_log_stream << "TwoRegionsNode;" << (mean1 - mean2) << ";" << std::endl;
	
	if ((mean1 - mean2) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


