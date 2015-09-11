#include "MinNodeFactory.hpp"
#include "MinNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

MinNodeFactory::NodePtr MinNodeFactory::createRandomNode(std::ostream* log_stream)
{
    float threshold = m_threshold_real_dist(m_rng);
	return std::make_unique<MinNode>(threshold, log_stream);
}


