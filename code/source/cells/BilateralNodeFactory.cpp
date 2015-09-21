#include "BilateralNodeFactory.hpp"
#include "BilateralNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

BilateralNodeFactory::NodePtr BilateralNodeFactory::createRandomNode(std::ostream* log_stream)
{
    float threshold = m_threshold_dist(m_rng);
	float sigmaColor = m_color_dist(m_rng);
	float sigmaSpace = m_space_dist(m_rng);
	cv::Point2f pixel = cv::Point2f(m_pixel_x_dist(m_rng), m_pixel_y_dist(m_rng));
	return std::make_unique<BilateralNode>(sigmaColor, sigmaSpace, pixel, threshold, log_stream);
}


