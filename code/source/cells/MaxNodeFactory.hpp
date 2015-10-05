#ifndef MAXNODEFACTORY_HPP
#define	MAXNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class MaxNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
public:
	MaxNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_real_dist(0.8, 1.)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_real_dist;

} ;

#endif	/* MINNODEFACTORY_HPP */

