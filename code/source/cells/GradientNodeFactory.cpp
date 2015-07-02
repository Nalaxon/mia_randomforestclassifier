#include "GradientNodeFactory.hpp"
#include "GradientNode.hpp"


std::unique_ptr<Node<CellLabel, cv::Mat>> GradientNodeFactory::createRandomNode() {
  double threshold = m_dist_threshold(m_rng);
  return std::unique_ptr<GradientNode>(new GradientNode(threshold));
}