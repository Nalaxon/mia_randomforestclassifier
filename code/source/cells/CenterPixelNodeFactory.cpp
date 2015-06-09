#include "CenterPixelNodeFactory.hpp"

CenterPixelNode* CenterPixelNodeFactory::createRandomNode() {
    int x = m_dist_cols(m_rng);
    int y = m_dist_rows(m_rng);
    return new CenterPixelNode(x, y);
}