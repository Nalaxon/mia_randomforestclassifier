#include "../base/NodeFactory.hpp"
#include "CellLabel.hpp"
#include "CenterPixelNode.hpp"
#include "PatchParameter.hpp"

#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class CenterPixelNodeFactory - 

class CenterPixelNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:

  CenterPixelNodeFactory(const PatchParameter& params)
  : m_dist_rows(0, params.patch_height - 1),
  m_dist_cols(0, params.patch_width - 1),
  m_dist_threshold(0. , 1.)
  {
  }

protected:
  virtual std::unique_ptr<Node<CellLabel, cv::Mat>> createRandomNode();

private:
  boost::random::uniform_int_distribution<> m_dist_rows, m_dist_cols;
  boost::random::uniform_real_distribution<> m_dist_threshold;

} ;

