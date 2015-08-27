#ifndef HAAR4WAVELETNODEFACTORY_HPP
#define HAAR4WAVELETNODEFACTORY_HPP

#include <limits>

#include "NodeFactory.hpp"

#include "PatchParameter.hpp"

#include "CellLabel.hpp"
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

class Haar4WaveletNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
  public:
	  Haar4WaveletNodeFactory(PatchParameter params) :
		  //m_params(params), m_dist_threshold(params.max_value*255., 2.*255.*(params.patch_width)*(params.patch_height)*params.max_value)
		  m_params(params), m_dist_threshold(params.max_value*255., 10000.)
	  {
	  }
    
  protected:
	  virtual NodePtr createRandomNode(std::ostream* log_stream);
	  virtual std::string get_ClassName() { return (typeid(*this)).name(); };

  private:
    PatchParameter m_params;
    boost::random::uniform_real_distribution<> m_dist_threshold;
};
#endif