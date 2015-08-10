#ifndef SUMNODEFACTORY_HPP
#define SUMNODEFACTORY_HPP

#include "NodeFactory.hpp"
#include "PatchParameter.hpp"
#include "CellLabel.hpp"

#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

/// class SumNodeFactory - 
class SumNodeFactory : public NodeFactory<CellLabel, cv::Mat>
{
public:

  SumNodeFactory(PatchParameter params)
    : m_dist_threshold(0, (params.patch_width - 1)*(params.patch_height - 1)*params.max_value)
  {
  }

protected:
	virtual NodePtr createRandomNode(std::ostream* log_stream);

private:
  boost::random::uniform_real_distribution<> m_dist_threshold;
};
#endif
