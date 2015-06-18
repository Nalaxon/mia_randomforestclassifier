#include "CenterPixelNodeFactory.hpp"

CenterPixelNodeFactory::NodePtr CenterPixelNodeFactory::createRandomNode() {
    int x = m_dist_cols(m_rng);
    int y = m_dist_rows(m_rng);
    float threshold = m_dist_threshold(m_rng);
    return std::make_unique<CenterPixelNode>(x, y, threshold);
}