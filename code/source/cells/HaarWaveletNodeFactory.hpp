#ifndef HAARWAVELETNODEFACTORY_HPP
#define HAARWAVELETNODEFACTORY_HPP

#include "NodeFactory.hpp"

#include "PatchParameter.hpp"

#include "CellLabel.hpp"
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class GradientNodeFactory - 
class HaarWaveletNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
  public:
    HaarWaveletNodeFactory(PatchParameter params);
    
  protected:
    virtual NodePtr createRandomNode();

  private:
    PatchParameter m_params;
    boost::random::uniform_real_distribution<> m_dist_threshold;
};
#endif