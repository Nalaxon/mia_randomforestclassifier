#include "SumNodeFactory.hpp"
#include "SumNode.hpp"

std::unique_ptr<Node<CellLabel, cv::Mat>> SumNodeFactory::createRandomNode() {
  double threshold = m_dist_threshold(m_rng);
  return std::unique_ptr<SumNode>(new SumNode( threshold));

}
