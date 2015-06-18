//
// Created by chri on 30.05.15.
//

#include <opencv2/imgproc/imgproc.hpp>

#include "SampleExtractor.hpp"

SampleExtractor::SampleExtractor(const cv::Mat& sample_image, const cv::Mat& ground_truth, unsigned int sample_size)
: m_rng(),
m_dist_col(0, sample_image.cols-1),
m_dist_row(0, sample_image.rows-1),
m_sample_size(sample_size),
m_ground_truth(ground_truth) 
{   
    cv::copyMakeBorder(sample_image, m_sample_image, sample_size / 2, sample_size / 2, sample_size / 2, sample_size / 2, cv::BORDER_REPLICATE);
}

std::tuple<cv::Mat, bool> SampleExtractor::extractRandomSample() {
    int rnd_col = m_dist_col(m_rng);
    int rnd_row = m_dist_row(m_rng);
    // note: since the roi is applied to the border image, the x and y positions are the same!
    cv::Rect roi(rnd_col, rnd_row, m_sample_size, m_sample_size);
    cv::Mat patch(m_sample_image, roi);

    bool is_foreground = m_ground_truth.at<uchar>(rnd_row, rnd_col) == 0;
    return std::make_tuple(patch, is_foreground);
}
