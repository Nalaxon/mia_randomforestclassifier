#include "TwoPixelNodeFactory.hpp"
#include "TwoPixelNode.hpp"

TwoPixelNodeFactory::NodePtr TwoPixelNodeFactory::createRandomNode()
{
    int x1 = m_dist_cols(m_rng);
    int y1 = m_dist_rows(m_rng);
    int x2 = m_dist_cols(m_rng);
    int y2 = m_dist_rows(m_rng);
    float threshold = m_dist_threshold(m_rng);
    return std::make_unique<TwoPixelNode>(x1, y1, x2, y2, threshold);
}