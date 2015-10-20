#include "Haar4WaveletNodeFactory.hpp"
#include "Haar4WaveletNode.hpp"

Haar4WaveletNodeFactory::NodePtr Haar4WaveletNodeFactory::createRandomNode(std::ostream* log_stream)
{
  double threshold = m_dist_threshold(m_rng);
  bool is_vertical = m_dist_orientation(m_rng) > 0;
  return std::make_unique<Haar4WaveletNode>(is_vertical, threshold, log_stream);
}