#include "TwoRegionsNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsNode::TwoRegionsNode(const cv::Rect& region1, const cv::Rect& region2, float threshold)
:m_region1(region1), m_region2(region2), m_threshold(threshold)
{    
}

TwoRegionsNode::Direction TwoRegionsNode::split(const cv::Mat& mat) const
{
    float mean1 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region1);
    float mean2 = ImageTools::mean_from_integral<float, 4, 2>(mat, m_region2);
    if ((mean1 - mean2) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


