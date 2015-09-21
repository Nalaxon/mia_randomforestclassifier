#include "MembraneProjectionNodeFactory.hpp"
#include "MembraneProjectionNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

MembraneProjectionNodeFactory::NodePtr MembraneProjectionNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double sum = m_sum_dist(m_rng);
	double mean = m_mean_dist(m_rng);
	double stdv = m_stdv_dist(m_rng);
	double median = m_median_dist(m_rng);
	double max = m_max_dist(m_rng);
	double min = m_min_dist(m_rng);
	
	double test = m_test_dist(m_rng);
	cv::Point2f pixel = cv::Point2f(m_pixel_x_dist(m_rng), m_pixel_y_dist(m_rng));

	return std::make_unique<MembraneProjectionNode>(test, pixel, sum, mean, stdv,
		median, max, min, log_stream);
}


