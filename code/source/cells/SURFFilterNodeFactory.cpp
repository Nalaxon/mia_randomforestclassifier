#include "SURFFilterNodeFactory.hpp"
#include "SURFFilterNode.hpp"

SURFFilterNodeFactory::SURFFilterNodeFactory(PatchParameter params) :
m_params(params),
m_dist_threshold(),
m_dist_orientation(-1,1)
{
  double max_value = (params.patch_width - 1)*(params.patch_height - 1)*params.max_value;
  //m_dist_threshold = boost::random::uniform_real_distribution<>(-1.0 / 3.0 * max_value, 2.0 / 3.0 * max_value);
  m_dist_threshold = boost::random::uniform_real_distribution<>(-1.0 / 3.0 * max_value, 100000.);
}

SURFFilterNodeFactory::NodePtr SURFFilterNodeFactory::createRandomNode(std::ostream* log_stream) {
  double threshold = m_dist_threshold(m_rng);
  bool is_vertical = m_dist_orientation(m_rng) > 0;
  return std::make_unique<SURFFilterNode>(is_vertical, threshold, log_stream);
}