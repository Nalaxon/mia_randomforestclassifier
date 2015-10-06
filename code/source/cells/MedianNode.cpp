#include "MedianNode.hpp"
#include "Tools/ImageTools.hpp"

MedianNode::Direction MedianNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
	float median = 0.0;
	int mat_size = (data[0]).rows*(data[0]).cols;
	
	cv::Mat row = data[2].reshape(1, mat_size), sorted;
	cv::sortIdx(row, sorted, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);

	if (mat_size % 2 != 0)
		median = row.at<float>(sorted.at<float>(mat_size / 2));
	else
		median = (row.at<float>(sorted.at<int>(mat_size / 2 - 1)) + 
		row.at<float>(sorted.at<int>(mat_size / 2 + 1))) * 0.5;


	if (m_log_stream != nullptr)
	  *m_log_stream << "MedianNode;" << median << ";" << std::endl;

	if (median < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


