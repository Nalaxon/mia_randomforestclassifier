#ifndef HESSIANNODEFACTORY_HPP
#define	HESSIANNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class HessianNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:
	HessianNodeFactory(const PatchParameter& params)
		: m_params(params), m_test_dist(0, 7),
		m_pixel_x_dist(0, params.patch_width - 1), m_pixel_y_dist(0, params.patch_height),
		m_module_dist(0.1, 7.5), m_trace_dist(-7., 5.), m_determine_dist(-25., 16.),
		m_firsteigen_dist(-1., 5.), m_secondeigen_dist(-11., 0.3), m_orientation_dist(0., 1.),
		m_sgned_dist(0.2, 70.), m_gnsed_dist(0., 2400)
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
  boost::random::uniform_real_distribution<> m_module_dist, m_trace_dist,
	  m_determine_dist, m_firsteigen_dist, m_secondeigen_dist,
	  m_orientation_dist, m_sgned_dist, m_gnsed_dist;

} ;

#endif	/* HESSIANNODEFACTORY_HPP */

