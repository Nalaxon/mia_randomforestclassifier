#include "SDNodeFactory.hpp"
#include "SDNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

SDNodeFactory::NodePtr SDNodeFactory::createRandomNode(std::ostream* log_stream)
{
	double threshold = m_threshold_real_dist(m_rng);
	return std::make_unique<SDNode>(threshold, log_stream);
}


