#include "MedianNode.hpp"
#include "Tools/ImageTools.hpp"
#include "tools\utils.hpp"

MedianNode::Direction MedianNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[2], roi);
    float median = 0.0;
	int mat_size = (mat).rows*(mat).cols;
	
    cv::Mat row = convert_to_row_vector<float>(mat), sorted;
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


