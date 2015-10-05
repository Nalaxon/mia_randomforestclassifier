#include "GaussPyrNodeFactory.hpp"
#include "GaussPyrNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

GaussPyrNodeFactory::NodePtr GaussPyrNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_real_dist(m_rng);
	int level = m_level_dist(m_rng);
	int test = m_test_dist(m_rng);
	return std::make_unique<GaussPyrNode>(level, test, threshold, log_stream);
}


