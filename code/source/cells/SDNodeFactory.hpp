#ifndef SDNODEFACTORY_HPP
#define	SDNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class SDNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:
	SDNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_real_dist(0.1, 0.4)
  {
  }
  
protected:
  
    virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_real_dist;

} ;

#endif	/* SDNODEFACTORY_HPP */

