#ifndef MINMAXNODEFACTORY_HPP
#define	MINMAXNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class MinMaxNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
	MinMaxNodeFactory(const PatchParameter& params)
		: m_params(params), m_test_dist(0., 1.), m_threshold_real_dist(0., 50.)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_real_dist;
  boost::random::uniform_real_distribution<> m_test_dist;

} ;

#endif	/* MINMAXNODEFACTORY_HPP */

