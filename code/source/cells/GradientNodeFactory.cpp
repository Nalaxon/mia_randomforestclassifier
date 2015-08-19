#include "GradientNodeFactory.hpp"
#include "GradientNode.hpp"


GradientNodeFactory::NodePtr GradientNodeFactory::createRandomNode(std::ostream* log_stream) {
  double threshold = m_dist_threshold(m_rng);
  return std::make_unique<GradientNode>(threshold, log_stream);
}