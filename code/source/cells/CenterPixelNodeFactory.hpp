#include "../base/NodeFactory.hpp"
#include "Label.hpp"
#include "CenterPixelNode.hpp"
#include "PatchParameter.hpp"

#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class CenterPixelNodeFactory - 

class CenterPixelNodeFactory : public NodeFactory<Label, cv::Mat>
{
public:

  CenterPixelNodeFactory(PatchParameter params)
  : m_params(params),
  m_dist_rows(0, params.patch_height - 1),
  m_dist_cols(0, params.patch_width - 1),
  m_dist_threshold(0. , 1.)
  {
  }

protected:
  virtual NodePtr createRandomNode();

private:
  PatchParameter m_params;
  boost::random::uniform_int_distribution<> m_dist_rows, m_dist_cols;
  boost::random::uniform_real_distribution<> m_dist_threshold;

} ;

