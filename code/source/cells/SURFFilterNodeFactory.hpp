#ifndef SURFFILTERNODEFACTORY_HPP
#define SURFFILTERNODEFACTORY_HPP

#include "NodeFactory.hpp"

#include "PatchParameter.hpp"

#include "CellLabel.hpp"
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class SURFFilterNodeFactory - 
class SURFFilterNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
  public:

    SURFFilterNodeFactory(PatchParameter params);
     
  protected:
    virtual std::unique_ptr<Node<CellLabel, cv::Mat>> createRandomNode();

  private:
    PatchParameter m_params;
    boost::random::uniform_real_distribution<> m_dist_threshold;
    boost::random::uniform_real_distribution<> m_dist_orientation;
};
#endif