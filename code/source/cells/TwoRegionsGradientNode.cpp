#include "TwoRegionsGradientNode.hpp"
#include "tools/ImageTools.hpp"

TwoRegionsGradientNode::Direction TwoRegionsGradientNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float TwoRegionsGradientNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat  mat = cv::Mat(data[1], roi);
    std::vector<cv::Mat> mat1(4), mat2(4);

    float mean1 = ImageTools::mean_from_integral<float>(mat, m_region1);
    float mean2 = ImageTools::mean_from_integral<float>(mat, m_region2);
    double sub = abs(mean1 - mean2);

    if (m_log_stream != nullptr)
        *m_log_stream << "TwoRegionsGradientNode;" << sub << std::endl;

    return sub;
}

void TwoRegionsGradientNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}