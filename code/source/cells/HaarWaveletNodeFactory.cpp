#include "HaarWaveletNodeFactory.hpp"
#include "HaarWaveletNode.hpp"

HaarWaveletNodeFactory::HaarWaveletNodeFactory(PatchParameter params) : 
m_params(params),
m_dist_threshold()
{
  double span = (params.patch_width - 1)*(params.patch_height - 1)*params.max_value / 2.0;
  m_dist_threshold = boost::random::uniform_real_distribution<>(-span, span);
}

HaarWaveletNodeFactory::NodePtr HaarWaveletNodeFactory::createRandomNode(std::ostream* log_stream) {
  double threshold = m_dist_threshold(m_rng);
  bool is_vertical = m_dist_threshold(m_rng) > 0;
  return std::make_unique<HaarWaveletNode>(is_vertical, threshold, log_stream);
}