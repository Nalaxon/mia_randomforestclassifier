#include "GradientNodeFactory.hpp"

#include "GradientNode.hpp"

GradientNodeFactory::NodePtr GradientNodeFactory::createRandomNode() {
  double threshold = m_dist_threshold(m_rng);
  return std::make_unique<GradientNode>(threshold);
}