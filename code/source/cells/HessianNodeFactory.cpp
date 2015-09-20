#include "HessianNodeFactory.hpp"
#include "HessianNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

HessianNodeFactory::NodePtr HessianNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double module = m_module_dist(m_rng);
	double trace = m_trace_dist(m_rng);
	double determine = m_determine_dist(m_rng);
	double orientation = m_orientation_dist(m_rng);
	double first = m_firsteigen_dist(m_rng);
	double second = m_secondeigen_dist(m_rng);
	double sgned = m_sgned_dist(m_rng);
	double gnsed = m_gnsed_dist(m_rng);
	double test = m_test_dist(m_rng);
	cv::Point2d pixel = cv::Point2d(m_pixel_x_dist(m_rng), m_pixel_y_dist(m_rng));

	return std::make_unique<HessianNode>(test, pixel, module, trace, determine,
		orientation, first, second, sgned, gnsed, log_stream);
}


