#include "TwoPixelGradientNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelGradientNode::Direction TwoPixelGradientNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	float sum = 0;
	for (unsigned int i = 0; i < m_x_1.size(); ++i)
	{
		const auto& pixel1 = data[4].at<float>(m_y_1[i], m_x_1[i]);
		const auto& pixel2 = data[4].at<float>(m_y_2[i], m_x_2[i]);
		if (pixel1 < pixel2)
			++sum;
	}

	if (m_log_stream != nullptr)
		*m_log_stream << "TwoPixelGradientNode;" << (sum / m_x_1.size()) << ";" << std::endl;
    
    if ((sum/ m_x_1.size()) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}