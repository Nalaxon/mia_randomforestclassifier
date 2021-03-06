//
// Created by chri on 30.05.15.
//

#ifndef MIA_2015_PROGRAM_HPP
#define MIA_2015_PROGRAM_HPP

#include "cells/CellLabel.hpp"
#include "base/Sample.hpp"
#include "base/RandomForest.hpp"

#include <string>
#include <tuple>
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
    
    using PathTuple = std::tuple<boost::filesystem::path, boost::filesystem::path>;

    Program();

    /** Destructor */
    virtual ~Program();

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
     * Defines wether bagging of the input samples will be enabled
     */
    bool m_bagging;
    
    /**
     * The number of trees that will be trained
     */
    unsigned int m_num_trees;
    
    /**
     * The maximal depth of each tree
     */
    unsigned int m_max_depth;
    
    /**
     * The minimum number of samples at which a node will be splitted again
     */
    unsigned int m_min_samples_per_node;
    
    /**
     * The number of feature tests to try at each split
     */
    unsigned int m_num_feature_tests;

    /**
    * Defines to which output stream the tree should be printed after training
    */
    std::ostream* m_tree_output_stream;

	/**
	* Defines to which output stream the nodes should be logged
	**/
	std::ostream* m_log_stream;

    /**
    * True if cross validation should be used, otherwise false
    */
    bool m_use_xvalidation;
    
    /**
     * The number of cross validation sets.
     */
    unsigned int m_num_xvalidation_sets;
    
    /**
     * Index of testing image which was not used for training
     */
    int m_test_image_index;

	/**
	* Path to directory for logging data. nothing will be stored if ""
	*/
	boost::filesystem::path m_log_path;

    /**
     * Creates the binary classification image (background=255, foreground=0).
     *
     * @param forest the random forest
     * @param image the input image
     * @return the classification image
     */
    cv::Mat classify_image(const RandomForest<CellLabel, std::vector<cv::Mat>, cv::Rect>& forest, const std::vector<cv::Mat>& images) const;

    /**
     * Extracts the training samples from the dataset.
     *
     * @param foreground_samples all foreground samples will be in there.
     */
	void extract_training_samples(std::vector<Sample<CellLabel, std::vector<cv::Mat>, cv::Rect>>& samples) const;

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
	std::vector<cv::Mat> prepare_image(const cv::Mat& image) const;
    
    PathTuple resolve_data_path(unsigned int id) const;

	double xvalidation(RandomForest<CellLabel, std::vector<cv::Mat>, cv::Rect> &forest, const std::vector < Sample<CellLabel, std::vector<cv::Mat>, cv::Rect>> &pure_samples, const unsigned int validations);

	int MAX_IMAGES = 30;

	cv::Mat segment_image(const cv::Mat& classify_image, const cv::Mat& prop_image, std::string options);
	cv::Mat watershed_image(const cv::Mat& classify_image, const cv::Mat& prop_image);
	void push_tuble(cv::Mat input, std::vector<cv::Mat>& prepared, int ddepth) const;
};


#endif //MIA_2015_PROGRAM_H
