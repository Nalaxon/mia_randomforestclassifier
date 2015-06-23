#include "TwoPixelGradientNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelGradientNode::Direction TwoPixelGradientNode::split(const cv::Mat& data) const
{
	
	int sum = 0;
	for (int i = 0; i < m_x_1.size(); ++i)
	{
		const auto& pixel1 = ImageTools::get_pixel<float, 4, 4>(data, m_y_1[i], m_x_1[i]);
		const auto& pixel2 = ImageTools::get_pixel<float, 4, 4>(data, m_y_2[i], m_x_2[i]);
		if (pixel1 < pixel2)
			++sum;
	}
    
    if (((float)sum/(float)m_x_1.size()) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}