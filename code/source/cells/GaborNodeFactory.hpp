#ifndef GABORODEFACTORY_HPP
#define	GABORNODEFACTORY_HPP

#include "PatchParameter.hpp"
#include "CellLabel.hpp"
#include "NodeFactory.hpp"

#include <boost/random.hpp>
#include <opencv2/core/core.hpp>

class GaborNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:
	GaborNodeFactory(const PatchParameter& params)
		: m_params(params), m_threshold_real_dist(0.4, 1.2),
		m_sig_th_lm_ps_dist(0., 1.2), m_gm_dist(0.005, 0.03)
  {
  }
  
protected:
  
    virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  PatchParameter m_params;
  boost::random::uniform_real_distribution<> m_threshold_real_dist;
  boost::random::uniform_real_distribution<> m_sig_th_lm_ps_dist;
  boost::random::uniform_real_distribution<> m_gm_dist;
} ;

#endif	/* GABORNODEFACTORY_HPP */

