#ifndef MEMBRANEPROJECTIONNODEFACTORY_HPP
#define	MEMBRANEPROJECTIONNODEFACTORY_HPP

#include "../base/NodeFactory.hpp"
#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "MembraneProjectionNode.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class MembraneProjectionNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:
	MembraneProjectionNodeFactory(const PatchParameter& params)
		: m_params(params), m_test_dist(0, 5),
		m_pixel_x_dist(0, params.patch_width - 1), m_pixel_y_dist(0, params.patch_height -1),
		m_sum_dist(100., 500.), m_mean_dist(3., 15.), m_stdv_dist(0., 16.),
		m_median_dist(-1., 5.), m_max_dist(0., 1.3), m_min_dist(2., 11.5)
  {
  }
  
protected:
  
    virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_int_distribution<> m_test_dist;
  boost::random::uniform_int_distribution<> m_pixel_x_dist;
  boost::random::uniform_int_distribution<> m_pixel_y_dist;
  boost::random::uniform_real_distribution<> m_sum_dist, m_mean_dist,
	  m_stdv_dist, m_median_dist, m_max_dist, m_min_dist;

} ;

#endif	/* MEMBRANEPROJECTIONNODEFACTORY_HPP */

