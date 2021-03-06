#ifndef HOGNODEFACTORY_HPP
#define	HOGNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class HoGNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:
	HoGNodeFactory(const PatchParameter& params)
  : m_params(params),m_threshold_dist(20., 22.)
  {
  }
  
protected:
  
    virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_dist;

} ;

#endif	/* HOGNODEFACTORY_HPP */

