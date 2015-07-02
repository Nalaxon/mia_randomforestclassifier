#ifndef TWOREGIONSGRADIENTNODEFACTORY_HPP
#define	TWOREGIONSGRADIENTNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class TwoRegionsGradientNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
	TwoRegionsGradientNodeFactory(const PatchParameter& params)
  : m_params(params), m_row_dist(0, params.patch_width-1), m_col_dist(0, params.patch_height-1), m_threshold_dist(0., 1.)
  {
  }
  
protected:
  
  virtual std::unique_ptr<Node<CellLabel, cv::Mat>> createRandomNode();

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_int_distribution<> m_row_dist, m_col_dist;
  boost::random::uniform_real_distribution<> m_threshold_dist;
  
  void adjust_region(cv::Rect& region);
} ;

#endif	/* TWOREGIONSGRADIENTNODEFACTORY_HPP */

