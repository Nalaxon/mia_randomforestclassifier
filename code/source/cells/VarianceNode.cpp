#include "VarianceNode.hpp"
#include "tools/ImageTools.hpp"

VarianceNode::Direction VarianceNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
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

float VarianceNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    cv::Mat psd, mat = cv::Mat(data[2], roi);
    cv::dft(mat.mul(mat), psd);
    float var = cv::mean(psd).val[0];

    if (m_log_stream != nullptr)
        *m_log_stream << "VarianceNode;" << m_threshold << ";" << var << "; " << std::endl;

    return var;
}

void VarianceNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}
