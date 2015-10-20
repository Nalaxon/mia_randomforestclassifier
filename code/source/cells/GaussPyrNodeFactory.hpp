#ifndef GAUSSPYRNODEFACTORY_HPP
#define	GAUSSPYRNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class GaussPyrNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:
	GaussPyrNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_real_dist(-0.01, 0.9),
		m_level_dist(1, params.patch_height / 2 - 1), m_test_dist(0, 1)
  {
  }
  
protected:
  
    virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  
  boost::random::uniform_real_distribution<> m_threshold_real_dist;
  boost::random::uniform_int_distribution<> m_level_dist;
  boost::random::uniform_int_distribution<> m_test_dist;

} ;

#endif	/* GAUSSPYRNODEFACTORY_HPP */

