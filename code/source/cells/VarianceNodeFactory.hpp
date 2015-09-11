#ifndef VARIANCENODEFACTORY_HPP
#define	VARIANCENODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class VarianceNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
	VarianceNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_real_dist(-0.01, 0.9)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_real_dist;

} ;

#endif	/* VARIANCENODEFACTORY_HPP */

