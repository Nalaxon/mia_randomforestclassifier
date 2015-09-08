#include "MinMaxNodeFactory.hpp"
#include "MinMaxNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

MinMaxNodeFactory::NodePtr MinMaxNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_real_dist(m_rng);
	bool test = m_test_dist(m_rng) > 0.5;
	return std::make_unique<MinMaxNode>(test, threshold, log_stream);
}


