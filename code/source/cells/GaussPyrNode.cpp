#include "GaussPyrNode.hpp"
#include "Tools/ImageTools.hpp"

GaussPyrNode::Direction GaussPyrNode::split(const cv::Mat& mat) const
{
	std::vector<cv::Mat> channels = ImageTools::extract_channels<4>(mat);
	
	cv::Mat act = channels[0], down, up, laplace;
	int act_size = channels[0].cols / 2;
	int level = 1;
	while ((act_size > 0) && (act_size % 2 == 0))
	{
		cv::pyrDown(act, down, cv::Size(act_size, act_size));
		cv::pyrUp(down, up, cv::Size(act_size * 2, act_size * 2));
		laplace = act - up;
		act_size = act_size / 2;
		act = down;
		if (level++ == m_level)
			break;
	}

	cv::Point2f pixel;
	float value;
	if (m_test == 0)
	{
		pixel = cv::Point2f(act.cols / 2, act.rows / 2);
		value = act.at<float>(pixel);
	}
	else
	{
		pixel = cv::Point2f(laplace.cols / 2, laplace.rows / 2);
		value = laplace.at<float>(pixel);
	}

	if (m_log_stream != nullptr)
		*m_log_stream << "GaussPyrNode;" << m_threshold << ";" << value << "; " << m_test << "; " << m_level << std::endl;

	if (value < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


