#ifndef SURFFILTERNODEFACTORY_HPP
#define SURFFILTERNODEFACTORY_HPP

#include "NodeFactory.hpp"

#include "PatchParameter.hpp"

#include "Label.hpp"
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class SURFFilterNodeFactory - 
class SURFFilterNodeFactory : public NodeFactory<Label, cv::Mat>
{
  public:

    SURFFilterNodeFactory(PatchParameter params);
     
  protected:
    virtual NodePtr createRandomNode();

  private:
    PatchParameter m_params;
    boost::random::uniform_real_distribution<> m_dist_threshold;
    boost::random::uniform_real_distribution<> m_dist_orientation;
};
#endif