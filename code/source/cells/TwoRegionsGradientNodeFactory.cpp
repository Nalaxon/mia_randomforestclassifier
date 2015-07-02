#include "TwoRegionsGradientNodeFactory.hpp"
#include "TwoRegionsGradientNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

std::unique_ptr<Node<CellLabel, cv::Mat>> TwoRegionsGradientNodeFactory::createRandomNode()
{
    cv::Rect region1(m_col_dist(m_rng), m_row_dist(m_rng), m_row_dist(m_rng), m_col_dist(m_rng));
    cv::Rect region2(m_col_dist(m_rng), m_row_dist(m_rng), m_row_dist(m_rng), m_col_dist(m_rng));
    adjust_region(region1);
    adjust_region(region2);
    float threshold = m_threshold_dist(m_rng);
    return std::unique_ptr<TwoRegionsGradientNode>(new TwoRegionsGradientNode(region1, region2, threshold));
}

void TwoRegionsGradientNodeFactory::adjust_region(cv::Rect& region)
{
    if ((region.x + region.width) >= m_params.patch_width)
    {
        region.width = m_params.patch_width - region.x - 1;
    }
    
    if ((region.y + region.height) >= m_params.patch_height)
    {
        region.height = m_params.patch_height - region.y - 1;
    }
}


