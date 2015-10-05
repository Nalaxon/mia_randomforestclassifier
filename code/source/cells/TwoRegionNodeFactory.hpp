#ifndef TWOREGIONNODEFACTORY_HPP
#define	TWOREGIONNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class TwoRegionNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
public:
  TwoRegionNodeFactory(const PatchParameter& params) 
	  : m_params(params), m_row_start_dist(0, params.patch_width - 2),
	  m_col_start_dist(0, params.patch_height - 2),
	  m_row_width_dist(1, params.patch_width - 1),
	  m_col_height_dist(1, params.patch_height - 1),
	  m_threshold_dist(0., 1.)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_int_distribution<> m_row_start_dist, m_col_start_dist,
	  m_row_width_dist, m_col_height_dist;
  boost::random::uniform_real_distribution<> m_threshold_dist;

} ;

#endif	/* TWOREGIONNODEFACTORY_HPP */

