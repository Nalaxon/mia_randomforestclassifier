//
// Created by chri on 30.05.15.
//

#include "SampleExtractor.h"

SampleExtractor::SampleExtractor(cv::Mat& sample_image, cv::Mat& ground_truth, unsigned int sample_size)
        : m_rng(),
          // we do not want to have border issues!
          m_dist_col(sample_size, sample_image.cols - sample_size),
          m_dist_row(sample_size, sample_image.rows - sample_size),
          m_sample_size(sample_size),
          m_sample_image(sample_image),
          m_ground_truth(ground_truth)
{
}

std::tuple<cv::Mat, bool> SampleExtractor::extractRandomSample()
{
    int rnd_col = m_dist_col(m_rng);
    int rnd_row = m_dist_row(m_rng);
    cv::Mat patch(m_sample_image, cv::Rect(rnd_col, rnd_row, m_sample_size, m_sample_size));

    bool is_foreground = m_ground_truth.at<uchar>(rnd_row, rnd_col) == 0;

    return std::make_tuple(patch, is_foreground);
}
