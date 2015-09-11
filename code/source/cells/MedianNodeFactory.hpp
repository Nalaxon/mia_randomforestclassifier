#ifndef MEDIANNODEFACTORY_HPP
#define	MEDIANNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class MedianNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
	MedianNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_real_dist(0., 1.)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_real_dist;

} ;

#endif	/* MEDIANNODEFACTORY_HPP */

