#include "CannyEdgeMoment00Node.hpp"
#include "Tools/ImageTools.hpp"

CannyEdgeMoment00Node::Direction CannyEdgeMoment00Node::split(const std::vector<cv::Mat>& data,
    const cv::Rect& roi) const
{
    cv::Mat detected_edges, mat = cv::Mat(data[0], roi);
    mat.convertTo(detected_edges, CV_8UC1, 255.);
	
	cv::Canny(detected_edges, detected_edges, m_canny_threshold, m_canny_threshold * 3, 3);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	std::vector<cv::Moments> mu(contours.size());
	double area = 0.0;
	for (int i = 0; i < contours.size(); i++)
		area = moments(contours[i], false).m00;
	if (m_log_stream != nullptr)
	  *m_log_stream << "CannyEdgeMoment00Node;" << area << ";" << std::endl;

	if (area < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}


