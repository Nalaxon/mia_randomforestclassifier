#include "MaxNodeFactory.hpp"
#include "MaxNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

MaxNodeFactory::NodePtr MaxNodeFactory::createRandomNode(std::ostream* log_stream)
{
    float threshold = m_threshold_real_dist(m_rng);
	return std::make_unique<MaxNode>(threshold, log_stream);
}


