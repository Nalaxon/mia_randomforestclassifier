#include "SURFFilterNodeFactory.hpp"
#include "SURFFilterNode.hpp"

SURFFilterNodeFactory::SURFFilterNodeFactory(PatchParameter params) :
m_params(params),
m_dist_threshold(),
m_dist_orientation(-1,1)
{
  double max_value = (params.patch_width - 1)*(params.patch_height - 1)*params.max_value;
  m_dist_threshold = boost::random::uniform_real_distribution<>(-1.0 / 3.0 * max_value, 2.0 / 3.0 * max_value);
}

std::unique_ptr<Node<CellLabel, cv::Mat>> SURFFilterNodeFactory::createRandomNode() {
  double threshold = m_dist_threshold(m_rng);
  bool is_vertical = m_dist_orientation(m_rng) > 0;
  return std::unique_ptr<SURFFilterNode>(new SURFFilterNode(is_vertical, threshold));
}