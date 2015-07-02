#include "HaarWaveletNodeFactory.hpp"
#include "HaarWaveletNode.hpp"

HaarWaveletNodeFactory::HaarWaveletNodeFactory(PatchParameter params) : 
m_params(params),
m_dist_threshold()
{
  double span = (params.patch_width - 1)*(params.patch_height - 1)*params.max_value / 2.0;
  m_dist_threshold = boost::random::uniform_real_distribution<>(-span, span);
}

std::unique_ptr<Node<CellLabel, cv::Mat>> HaarWaveletNodeFactory::createRandomNode() {
  double threshold = m_dist_threshold(m_rng);
  bool is_vertical = m_dist_threshold(m_rng) > 0;

  return std::unique_ptr<HaarWaveletNode>(new HaarWaveletNode(is_vertical, threshold));
}