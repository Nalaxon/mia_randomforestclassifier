#include "TwoPixelGradientNode.hpp"

#include "tools/ImageTools.hpp"

TwoPixelGradientNode::Direction TwoPixelGradientNode::split(const cv::Mat& data) const
{
	// create gradient
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;
	int blur_kernel_size = 15;
	cv::Mat blurred;
	cv::vector<cv::Mat> channels = ImageTools::extract_channels<4>(data);
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y, grad_abs;
	cv::Sobel(channels[3], grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);
	/// Gradient Y
	cv::Sobel(blurred, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);
	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad_abs);
	cv::Mat grad_abs_f;
	grad_abs.convertTo(grad_abs_f, CV_32F, 1.0f / 255.0f);

	int sum = 0;
	for (unsigned int i = 0; i < m_x_1.size(); ++i)
	{
		const auto& pixel1 = grad_abs_f.at<float>(m_y_1[i], m_x_1[i]);
		const auto& pixel2 = grad_abs_f.at<float>(m_y_1[i], m_x_1[i]);
		if (pixel1 < pixel2)
			++sum;
	}

	if (m_log_stream != nullptr)
		*m_log_stream << "TwoPixelGradientNode;" << (static_cast<float>(sum) / m_x_1.size()) << ";" << std::endl;
    
    if ((static_cast<float>(sum)/ m_x_1.size()) < m_threshold) {
        return Node::Direction::LEFT;
    } else {
        return Node::Direction::RIGHT;
    }
}