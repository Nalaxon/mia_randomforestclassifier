//
// Created by chri on 30.05.15.
//

#ifndef MIA_2015_PROGRAM_HPP
#define MIA_2015_PROGRAM_HPP

#include <string>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

/**
 * This class contains the logic of the program.
 */
class Program
{
public:
    /**
     * Runs the program logic.
     *
     * @param argc from main
     * @param argv from main
     */
    int run(int argc, char** argv);

private:

    /**
     * The path to dataset directory
     */
    boost::filesystem::path m_dataset_path;

    /**
     * Specifies how many samples should be extracted per training image
     */
    unsigned int m_num_samples_per_image;

    /**
     * The size of the samples. Each sample will be (sample_size x sample_size)
     */
    unsigned int m_sample_size;

    /**
     * Creates the binary classification image (background=255, foreground=0).
     *
     * @param forest the random forest
     * @param image the input image
     * @return the classification image
     */
    cv::Mat classify_image(CvRTrees& forest, const cv::Mat& image) const;

    /**
     * Extracts the training samples from the dataset.
     *
     * @param foreground_samples all foreground samples will be in there.
     * @param background_samples all background samples will be in there
     */
    void extract_training_samples(std::vector<cv::Mat>& foreground_samples,
                                  std::vector<cv::Mat>& background_samples) const;

    /**
     * Reads the configuration from the commandline.
     *
     * @param argc from main
     * @param argv from main
     */
    bool parse_command_line(int argc, char** argv);

    /**
     * Prepares an input image for better training/classification. It will convert the image to grayscale and do
     * histogram equalization.
     */
    void prepare_image(cv::Mat& image) const;
};


#endif //MIA_2015_PROGRAM_H
