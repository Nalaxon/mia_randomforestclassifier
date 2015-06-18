#ifndef TWOREGIONNODEFACTORY_HPP
#define	TWOREGIONNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class TwoRegionNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
  TwoRegionNodeFactory(const PatchParameter& params) 
  : m_params(params), m_row_dist(0, params.patch_width-1), m_col_dist(0, params.patch_height-1), m_threshold_dist(0., 1.)
  {
  }
  
protected:
  
  virtual NodePtr createRandomNode();

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_int_distribution<> m_row_dist, m_col_dist;
  boost::random::uniform_real_distribution<> m_threshold_dist;
  
  void adjust_region(cv::Rect& region);
} ;

#endif	/* TWOREGIONNODEFACTORY_HPP */

