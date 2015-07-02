#include "TwoPixelGradientNodeFactory.hpp"
#include "TwoPixelGradientNode.hpp"

std::unique_ptr<Node<CellLabel, cv::Mat>> TwoPixelGradientNodeFactory::createRandomNode()
{
	std::vector<unsigned int> x1, x2, y1, y2;

	for (int i = 0; i < 5; ++i)
	{
		x1.push_back(m_dist_cols(m_rng));
		y1.push_back(m_dist_rows(m_rng));
		x2.push_back(m_dist_cols(m_rng));
		y2.push_back(m_dist_rows(m_rng));
	}
    
  float threshold = m_dist_threshold(m_rng);
  return std::unique_ptr<TwoPixelGradientNode>(new TwoPixelGradientNode(x1, y1, x2, y2, threshold));
}
