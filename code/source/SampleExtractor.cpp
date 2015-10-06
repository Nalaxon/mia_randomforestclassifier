//
// Created by chri on 30.05.15.
//

#include <opencv2/imgproc/imgproc.hpp>

#include "SampleExtractor.hpp"

SampleExtractor::SampleExtractor(const std::vector<cv::Mat>& sample_image, const cv::Mat& ground_truth, unsigned int sample_size)
: m_rng(),
m_dist_col(0, sample_image[0].cols-1),
m_dist_row(0, sample_image[0].rows-1),
m_sample_size(sample_size),
m_ground_truth(ground_truth) 
{
    for (int i = 0; i < sample_image.size(); ++i)
    {
        cv::Mat tmp;
        cv::copyMakeBorder(sample_image[i], tmp, sample_size / 2, sample_size / 2, sample_size / 2, sample_size / 2, cv::BORDER_REPLICATE);
        m_sample_image.push_back(tmp.clone());
    }
    
}

std::tuple<std::vector<cv::Mat>, bool> SampleExtractor::extractRandomSample(cv::Rect& roi) {
    int rnd_col = m_dist_col(m_rng);
    int rnd_row = m_dist_row(m_rng);
    // note: since the roi is applied to the border image, the x and y positions are the same!
    roi = cv::Rect(rnd_col, rnd_row, m_sample_size, m_sample_size);
	std::vector<cv::Mat> patch;
	for (int i = 0; i < m_sample_image.size(); ++i)
	{
		patch.push_back(cv::Mat(m_sample_image[i], roi));
	}
    bool is_foreground = m_ground_truth.at<uchar>(rnd_row, rnd_col) == 0;
    return std::make_tuple(patch, is_foreground);
}
