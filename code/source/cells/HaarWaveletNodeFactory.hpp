#ifndef HAARWAVELETNODEFACTORY_HPP
#define HAARWAVELETNODEFACTORY_HPP

#include "NodeFactory.hpp"

#include "PatchParameter.hpp"

#include "CellLabel.hpp"
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class GradientNodeFactory - 
class HaarWaveletNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

  public:
    HaarWaveletNodeFactory(PatchParameter params);
    
  protected:
      virtual NodePtr createRandomNode(std::ostream* log_stream);
	  virtual std::string get_ClassName() { return (typeid(*this)).name(); };

  private:
    PatchParameter m_params;
    boost::random::uniform_real_distribution<> m_dist_threshold;
};
#endif