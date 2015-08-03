#include "HoGNodeFactory.hpp"
#include "HoGNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

HoGNodeFactory::NodePtr HoGNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_dist(m_rng);
	return std::make_unique<HoGNode>(threshold, log_stream);
}


