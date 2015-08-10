#include "SumNodeFactory.hpp"
#include "SumNode.hpp"

SumNodeFactory::NodePtr SumNodeFactory::createRandomNode(std::ostream* log_stream) {
  double threshold = m_dist_threshold(m_rng);
  return std::make_unique<SumNode>(threshold, nullptr);
}
