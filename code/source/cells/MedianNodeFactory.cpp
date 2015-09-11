#include "MedianNodeFactory.hpp"
#include "MedianNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

MedianNodeFactory::NodePtr MedianNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_real_dist(m_rng);
	return std::make_unique<MedianNode>(threshold, log_stream);
}


