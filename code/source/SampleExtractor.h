//
// Created by chri on 30.05.15.
//

#ifndef MIA_2015_SAMPLEEXTRACTOR_H
#define MIA_2015_SAMPLEEXTRACTOR_H

#include <tuple>
#include <stddef.h>
#include <opencv2/core/core.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

/**
 * Used to extract samples from the dataset.
 */
class SampleExtractor
{
public:

    /**
     * Constructor.
     *
     * @param sample_image the sample image
     * @param ground_truth the image containing the ground_truth for the sample image
     * @param the size of the samples
     */
    SampleExtractor(cv::Mat& sample_image, cv::Mat& ground_truth, unsigned int sample_size);

    /**
     * Extracts a random sample from the sample image.
     *
     * @return a tuple containing the sample and a boolean indicating the class (foreground=true, background=false)
     */
    std::tuple<cv::Mat, bool> extractRandomSample();

private:

    /**
     * The random device.
     */
    boost::random::random_device m_rng;

    /**
     * The uniform distribution for the column index.
     */
    boost::random::uniform_int_distribution<> m_dist_col;

    /**
     * The uniform distribution for the row index.
     */
    boost::random::uniform_int_distribution<> m_dist_row;

    /**
     * The size of the samples.
     */
    unsigned int m_sample_size;

    /**
     * The sample image.
     */
    cv::Mat& m_sample_image;

    /**
     * The ground truth image.
     */
    cv::Mat& m_ground_truth;
};


#endif //MIA_2015_SAMPLEEXTRACTOR_H
