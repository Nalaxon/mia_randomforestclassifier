#ifndef TWOPIXELNODEFACTORY_HPP
#define TWOPIXELNODEFACTORY_HPP

#include "../base/NodeFactory.hpp"
#include "CellLabel.hpp"
#include "CenterPixelNode.hpp"
#include "PatchParameter.hpp"

#include <opencv2/opencv.hpp>
#include <boost/random.hpp>


class TwoPixelNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>
{
    using SampleType = Sample < CellLabel, std::vector<cv::Mat>, cv::Rect >;

public:

  TwoPixelNodeFactory(PatchParameter params)
  : m_dist_rows(0, params.patch_height - 1),
  m_dist_cols(0, params.patch_width - 1),
  m_dist_threshold(0., 1.)
  {
  }

protected:
    virtual NodePtr createRandomNode(std::ostream* log_stream);
	virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  boost::random::uniform_int_distribution<> m_dist_rows, m_dist_cols;
  boost::random::uniform_real_distribution<> m_dist_threshold;
} ;


#endif