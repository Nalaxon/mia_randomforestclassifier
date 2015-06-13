#include "TwoPixelNodeFactory.hpp"
#include "TwoPixelNode.hpp"

TwoPixelNodeFactory::NodePtr TwoPixelNodeFactory::createRandomNode()
{
    int x1 = m_dist_cols(m_rng);
    int y1 = m_dist_rows(m_rng);
    int x2 = m_dist_cols(m_rng);
    int y2 = m_dist_rows(m_rng);
    return std::unique_ptr<TwoPixelNode>(new TwoPixelNode(x1, y1, x2, y2));
}