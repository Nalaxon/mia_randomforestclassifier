#include "CenterPixelNodeFactory.hpp"

std::unique_ptr<Node<CellLabel, cv::Mat>> CenterPixelNodeFactory::createRandomNode() {
    int x = m_dist_cols(m_rng);
    int y = m_dist_rows(m_rng);
    float threshold = m_dist_threshold(m_rng);
    return std::unique_ptr<CenterPixelNode>(new CenterPixelNode(x, y, threshold) );
}