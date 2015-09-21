#ifndef BILATERALNODEFACTORY_HPP
#define	BILATERALNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class BilateralNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
	BilateralNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_dist(0.1, 0.9), m_space_dist(50., 100.),
		m_color_dist(0.01, 1.2), m_pixel_x_dist(0, params.patch_width - 1),
		m_pixel_y_dist(0, params.patch_height - 1)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_dist;
  boost::random::uniform_int_distribution<> m_pixel_x_dist, m_pixel_y_dist;
  boost::random::uniform_real_distribution<> m_space_dist;
  boost::random::uniform_real_distribution<> m_color_dist;

} ;

#endif	/* BILATERALNODEFACTORY_HPP */

