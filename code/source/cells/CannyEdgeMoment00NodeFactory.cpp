#include "CannyEdgeMoment00NodeFactory.hpp"
#include "CannyEdgeMoment00Node.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

CannyEdgeMoment00NodeFactory::NodePtr CannyEdgeMoment00NodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_real_dist(m_rng);
	double canny_threshold = m_threshold_dist(m_rng);
	return std::make_unique<CannyEdgeMoment00Node>(canny_threshold, threshold, log_stream);
}


