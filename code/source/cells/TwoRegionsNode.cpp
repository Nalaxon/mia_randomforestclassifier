#include "TwoRegionsNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsNode::Direction TwoRegionsNode::split(const std::vector<cv::Mat>& mat, const cv::Rect& roi) const
{
    float mean1 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region1);
    float mean2 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region2);
    
	if (m_log_stream != nullptr)
		*m_log_stream << "TwoRegionsNode;" << (mean1 - mean2) << ";" << mean1 << ";" << mean2 << ";"
		<< m_region1.x << ";" << m_region1.y << ";" << m_region1.width << ";" << m_region1.height << std::endl;
	
	if ((mean1 - mean2) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


