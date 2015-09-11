#include "VarianceNodeFactory.hpp"
#include "VarianceNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

VarianceNodeFactory::NodePtr VarianceNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_real_dist(m_rng);
	return std::make_unique<VarianceNode>(threshold, log_stream);
}


