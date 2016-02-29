#include "CannyEdgeNode.hpp"
#include "tools/ImageTools.hpp"

CannyEdgeNode::Direction CannyEdgeNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    

    if (calc_thresh(data, roi) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}

float CannyEdgeNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat detected_edges, mat = cv::Mat(data[0], roi);
    mat.convertTo(detected_edges, CV_8UC1, 255.);

    cv::Canny(detected_edges, detected_edges, m_canny_threshold, m_canny_threshold * 3, 3);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    float length = 0.;
    for (int i = 0; i < contours.size(); ++i)
        length += arcLength(contours[i], true);

    if (m_log_stream != nullptr)
        *m_log_stream << "CannyEdgeNode;" << m_threshold << ";" << length << ";" << std::endl;

    return length;
}

void CannyEdgeNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}


