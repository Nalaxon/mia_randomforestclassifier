#ifndef GRADIENTNODEFACTORY_HPP
#define GRADIENTNODEFACTORY_HPP

#include "NodeFactory.hpp"

#include "PatchParameter.hpp"

#include "CellLabel.hpp"
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class GradientNodeFactory - 
class GradientNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

  public:

    GradientNodeFactory(PatchParameter params) 
    : m_dist_threshold(0,(params.patch_width - 1)*(params.patch_height-1)*params.max_value)
    {
    }

  protected:
      virtual NodePtr createRandomNode(std::ostream* log_stream);
	  virtual std::string get_ClassName() { return (typeid(*this)).name(); };

  private:
    boost::random::uniform_real_distribution<> m_dist_threshold;
};
#endif