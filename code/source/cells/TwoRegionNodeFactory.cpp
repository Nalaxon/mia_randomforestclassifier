#include "TwoRegionNodeFactory.hpp"
#include "TwoRegionsNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"
#include "tools/ImageTools.hpp"

TwoRegionNodeFactory::NodePtr TwoRegionNodeFactory::createRandomNode(std::ostream* log_stream)
{
	cv::Rect region1(m_col_start_dist(m_rng), m_row_start_dist(m_rng),
		m_row_width_dist(m_rng), m_col_height_dist(m_rng));
	cv::Rect region2(m_col_start_dist(m_rng), m_row_start_dist(m_rng),
		m_row_width_dist(m_rng), m_col_height_dist(m_rng));
	ImageTools::adjust_region(region1, m_params.patch_height, m_params.patch_width);
	ImageTools::adjust_region(region2, m_params.patch_height, m_params.patch_width);
    float threshold = m_threshold_dist(m_rng);
 	return std::make_unique<TwoRegionsNode>(region1, region2, threshold, log_stream);
}



