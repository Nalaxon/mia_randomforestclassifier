#include "TwoRegionNodeFactory.hpp"
#include "TwoRegionsNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

TwoRegionNodeFactory::NodePtr TwoRegionNodeFactory::createRandomNode(std::ostream* log_stream)
{
    cv::Rect region1(m_col_dist(m_rng), m_row_dist(m_rng), m_row_dist(m_rng), m_col_dist(m_rng));
    cv::Rect region2(m_col_dist(m_rng), m_row_dist(m_rng), m_row_dist(m_rng), m_col_dist(m_rng));
    adjust_region(region1);
    adjust_region(region2);
    float threshold = m_threshold_dist(m_rng);
	return std::make_unique<TwoRegionsNode>(region1, region2, threshold, log_stream);
}

void TwoRegionNodeFactory::adjust_region(cv::Rect& region)
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


