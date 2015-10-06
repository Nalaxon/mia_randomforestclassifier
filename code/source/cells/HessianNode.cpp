#include "HessianNode.hpp"
#include "Tools/ImageTools.hpp"

HessianNode::Direction HessianNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	double result = 0.;
	// create gradient
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;
	int blur_kernel_size = 15;

	cv::Mat grad_x, grad_y, grad_xx, grad_yy, grad_xy;
	std::vector<cv::Mat> grads;
	//Gradients:
	//X
	cv::Sobel(data[2], grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_REPLICATE);
	//XX
	cv::Sobel(grad_x, grad_xx, ddepth, 1, 0, 3, scale, delta, cv::BORDER_REPLICATE);
	normalizeImage(grad_xx);
	grads.push_back(grad_xx);
	//XY
	cv::Sobel(grad_x, grad_xy, ddepth, 0, 1, 3, scale, delta, cv::BORDER_REPLICATE);
	normalizeImage(grad_xy);
	grads.push_back(grad_xy);
	//Y
	cv::Sobel(data[2], grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_REPLICATE);
	//YY
	cv::Sobel(grad_y, grad_yy, ddepth, 0, 1, 3, scale, delta, cv::BORDER_REPLICATE);
	normalizeImage(grad_yy);
	grads.push_back(grad_yy);

	std::string test;
	double thresh = 0.;
	switch (m_test){
	case 0:
	{
		test = "module";
		result = calc_module(grads);
		thresh = m_module;
		//break;
	}
	case 1:
	{
		test = "trace";
		result = calc_trace(grads);
		thresh = m_trace;
		//break;
	}
	case 2:
	{
		test = "determine";
		result = calc_determine(grads);
		thresh = m_determine;
		//break;
	}
	case 3:
	{
		test = "firsteigen";
		result = calc_firsteigenvalue(grads);
		thresh = m_first;
		//break;
	}
	case 4:
	{
		test = "secondeigen";
		result = calc_secondeigenvalue(grads);
		thresh = m_second;
		//break;
	}
	case 5:
	{
		test = "orientation";
		result = calc_orientation(grads);
		thresh = m_orientation;
		//break;
	}
	case 6:
	{
		test = "gnsed";
		result = calc_gnsed(grads);
		thresh = m_gnsed;
		//break;
	}
	case 7:
	{
		test = "sgned";
		result = calc_sgned(grads);
		thresh = m_sgned;
		//break;
	}
	}

	if (m_log_stream != nullptr)
		*m_log_stream << "Hessian;" << test << ";" << thresh << ";" << result << "; " << std::endl;

	if (result < thresh)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}

double HessianNode::calc_module(const std::vector<cv::Mat> &grads) const
{
	return std::sqrtf(grads[0].at<float>(m_pixel) * grads[0].at<float>(m_pixel) +
		grads[1].at<float>(m_pixel) * grads[1].at<float>(m_pixel) +
		grads[2].at<float>(m_pixel) * grads[2].at<float>(m_pixel));
}

double HessianNode::calc_trace(const std::vector<cv::Mat> &grads) const
{
	return grads[0].at<float>(m_pixel) + grads[2].at<float>(m_pixel);
}

double HessianNode::calc_determine(const std::vector<cv::Mat> &grads) const
{
	return grads[0].at<float>(m_pixel) * grads[2].at<float>(m_pixel) -
		grads[1].at<float>(m_pixel) * grads[1].at<float>(m_pixel);
}

double HessianNode::calc_firsteigenvalue(const std::vector<cv::Mat> &grads) const
{
	double half = 0.5 * (grads[0].at<float>(m_pixel) +grads[2].at<float>(m_pixel));
	double sqrt = std::sqrtf((4. * grads[1].at<float>(m_pixel)*grads[1].at<float>(m_pixel) +
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))*
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))) * 0.5);

	return half + sqrt;
}

double HessianNode::calc_secondeigenvalue(const std::vector<cv::Mat> &grads) const
{
	double half = 0.5 * (grads[0].at<float>(m_pixel) +grads[2].at<float>(m_pixel));
	double sqrt = std::sqrtf((4. * grads[1].at<float>(m_pixel)*grads[1].at<float>(m_pixel) +
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))*
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))) * 0.5);

	return half - sqrt;
}

double HessianNode::calc_orientation(const std::vector<cv::Mat> &grads) const
{
	return 0.5 * std::acosf((4. * grads[1].at<float>(m_pixel)*grads[1].at<float>(m_pixel) +
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))*
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))));
}

double HessianNode::calc_gnsed(const std::vector<cv::Mat> &grads) const
{
	return ((grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))*
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel)))*
		((grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))*
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel)) +
		4. * grads[1].at<float>(m_pixel)*grads[1].at<float>(m_pixel));
}

double HessianNode::calc_sgned(const std::vector<cv::Mat> &grads) const
{
	return std::powf(1., 3./2.) *
		((grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel))*
		(grads[0].at<float>(m_pixel) -grads[2].at<float>(m_pixel)) +
		4. * grads[1].at<float>(m_pixel)*grads[1].at<float>(m_pixel));
}

void HessianNode::normalizeImage(cv::Mat &image) const
{
	double minVal, maxVal;
	cv::minMaxLoc(image, &minVal, &maxVal);
	image.convertTo(image, image.type(), 1. / (maxVal - minVal), minVal / (maxVal - minVal));
}

