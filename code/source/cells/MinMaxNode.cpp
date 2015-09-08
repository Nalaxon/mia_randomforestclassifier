#include "MinMaxNode.hpp"
#include "Tools/ImageTools.hpp"

MinMaxNode::Direction MinMaxNode::split(const cv::Mat& mat) const
{
	double min_val, extrema;
	if (m_test == TEST_MAX)
		cv::minMaxLoc(mat, &min_val, &extrema);
	else
		cv::minMaxLoc(mat, &extrema);

	if (m_log_stream != nullptr)
		*m_log_stream << "CannyEdgeMoment00Node;" << extrema << ";" << std::endl;

	if (extrema < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


