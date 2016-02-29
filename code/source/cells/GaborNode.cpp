#include "GaborNode.hpp"
#include "tools/ImageTools.hpp"

GaborNode::Direction GaborNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float GaborNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat mat = cv::Mat(data[0], roi);
    int kernel_size = mat.cols / 3;
    cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), m_sig, m_th, m_lm, m_gm, m_ps);

    cv::Mat dest;
    cv::filter2D(mat, dest, CV_32F, kernel);

    float mean = std::abs(cv::mean(dest).val[0]);

    if (m_log_stream != nullptr)
        *m_log_stream << "GaborNode;" << mean << ";" << std::endl;

    return mean;
}

void GaborNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}


