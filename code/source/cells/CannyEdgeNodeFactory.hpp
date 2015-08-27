#ifndef CANNYEDGENODEFACTORY_HPP
#define	CANNYEDGENODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class CannyEdgeNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:
	CannyEdgeNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_dist(50, 150), m_threshold_real_dist(400., 1000.)
  {
  }
  
protected:
  
	virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_int_distribution<> m_threshold_dist;
  boost::random::uniform_real_distribution<> m_threshold_real_dist;

} ;

#endif	/* CANNYEDGENODEFACTORY_HPP */

