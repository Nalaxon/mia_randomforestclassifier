#include "CannyEdgeNodeFactory.hpp"
#include "CannyEdgeNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

CannyEdgeNodeFactory::NodePtr CannyEdgeNodeFactory::createRandomNode()
{
    double threshold = m_threshold_real_dist(m_rng);
	double canny_threshold = m_threshold_dist(m_rng);
	return std::make_unique<CannyEdgeNode>(canny_threshold, threshold);
}


