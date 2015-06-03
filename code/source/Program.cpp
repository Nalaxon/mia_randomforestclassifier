//
// Created by chri on 30.05.15.
//

#include "Program.hpp"
#include "SampleExtractor.hpp"
#include "tools/utils.hpp"

#include <iomanip>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/program_options.hpp>
#include <boost/random.hpp>


//----------------------------------------------------------------------------------------------------------------------
int Program::run(int argc, char** argv)
{
    if (!parse_command_line(argc, argv))
    {
        return EXIT_FAILURE;
    }

    std::vector<cv::Mat> foreground_samples, background_samples;
    extract_training_samples(foreground_samples, background_samples);

    // the following is just tmp code using the OpenCv Random Forest!


    // select sum test samples
    unsigned long num_test_samples = 100;
    std::vector<cv::Mat> test_foreground_samples, test_background_samples;
    for (unsigned int sample = 0; sample < num_test_samples / 2; ++sample)
    {
        test_foreground_samples.push_back(foreground_samples.back());
        foreground_samples.pop_back();
    }
    for (unsigned int sample = 0; sample < num_test_samples / 2; ++sample)
    {
        test_background_samples.push_back(background_samples.back());
        background_samples.pop_back();
    }

    // constrcut the training data matrix.
    unsigned long num_samples = foreground_samples.size() + background_samples.size();
    unsigned int num_sample_features = m_sample_size * m_sample_size;
    cv::Mat training_data(num_samples, num_sample_features, CV_32FC1);
    cv::Mat training_labels(num_samples, 1, CV_32FC1);
    unsigned int sample = 0;
    for (auto foreground_sample : foreground_samples)
    {
        convert_to_row_vector<float>(foreground_sample).copyTo(training_data.row(sample));
        training_labels.at<float>(sample++, 0) = 1.;
    }
    for (auto background_sample : background_samples)
    {
        convert_to_row_vector<float>(background_sample).copyTo(training_data.row(sample));
        training_labels.at<float>(sample++, 0) = 0.;
    }

    // the prior probabilites for both classes
    float priors[] = {((float) background_samples.size()) / num_samples,
                      ((float) foreground_samples.size()) / num_samples};

    // tell the implementation details about the problem (classification)
    cv::Mat var_type(num_sample_features + 1, 1, CV_8U);
    var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL)); // inputs are numerical
    var_type.at<uchar>(num_sample_features, 0) = CV_VAR_CATEGORICAL; // classification problem

    // params for the forest
    CvRTParams rt_params(15, // max depth
                         5, // min sample count
                         0, // regression param
                         false, // compute surrogate split
                         2, // max categories
                         priors, // prior probabilities
                         false, // calculate variable importance
                         50, // number of features to select
                         100, // max number of trees
                         0.01f, // forest accuracy
                         CV_TERMCRIT_ITER | CV_TERMCRIT_EPS);


    std::cout << "Start training..." << std::endl;
    CvRTrees forest;
    forest.train(training_data, CV_ROW_SAMPLE, training_labels,
                 cv::Mat(), cv::Mat(), var_type, cv::Mat(), rt_params);

    std::cout << "Training done!" << std::endl;

    // test the forest
    unsigned int num_correct = 0;
    for (auto foreground_sample : test_foreground_samples)
    {
        double prediction = forest.predict(convert_to_row_vector<float>(foreground_sample));
        if (prediction == 1.0)
        {
            ++num_correct;
        }
    }
    for (auto background_sample : test_background_samples)
    {
        double prediction = forest.predict(convert_to_row_vector<float>(background_sample));
        if (prediction == 0.0)
        {
            ++num_correct;
        }
    }

    std::cout << num_correct << " correct classification of " << num_test_samples << std::endl;
    
    cv::Mat test_image = cv::imread("../../data/MIA_KU_2015_DataSet/train-volume0001.tif", CV_LOAD_IMAGE_COLOR);
    cv::namedWindow("inputwindow", CV_WINDOW_AUTOSIZE);
    cv::imshow("inputwindow", test_image);
    if (test_image.channels() != 1)
    {
        cv::cvtColor(test_image, test_image, CV_BGR2GRAY);
    }
    cv::equalizeHist(test_image, test_image);
    test_image.convertTo(test_image, CV_32FC1, 1 / 255.);
    cv::Mat classification_image = classify_image(forest, test_image);

    cv::namedWindow("resultwindow", CV_WINDOW_AUTOSIZE);
    cv::imshow("resultwindow", classification_image);
    cv::waitKey(0);

    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
bool Program::parse_command_line(int argc, char** argv)
{
    namespace po = boost::program_options;

    // define the supported program options
    po::options_description option_desc("Supported options");
    option_desc.add_options()
            ("help", "display this help message")

            ("dataset", po::value<std::string>()->required(),
             "path to the directory of the dataset")

            ("num_samples", po::value<unsigned int>()->default_value(30 * 30),
             "the number of samples that will be extracted from each training image")

            ("sample_size", po::value<unsigned int>()->default_value(30),
             "the size of the samples. (Each sample will be (sample_size x sample_size)");

    // parse commandline input
    po::variables_map given_options;
    try
    {
        po::store(po::parse_command_line(argc, argv, option_desc),
                  given_options);

        // handle help before notify!
        if (given_options.count("help"))
        {
            std::cout << option_desc << std::endl;
            return false;
        }

        // notify will validate the input
        po::notify(given_options);

    } catch (const po::error& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << option_desc;
        return false;
    }

    // read dataset path
    namespace fs = boost::filesystem;
    fs::path dataset_path(given_options["dataset"].as<std::string>());
    m_dataset_path = dataset_path.string();

    m_num_samples_per_image = given_options["num_samples"].as<unsigned int>();

    m_sample_size = given_options["sample_size"].as<unsigned int>();

    // handle further options here if needed

    return true;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void Program::extract_training_samples(std::vector<cv::Mat>& foreground_samples,
                                       std::vector<cv::Mat>& background_samples) const
{
    std::vector<std::string> foreground_image_paths, background_image_paths;
    #pragma omp parallel for
    for (int i_file = 1; i_file <= 30; ++i_file)
    {
        std::ostringstream volume_file_name, truth_file_name;
        volume_file_name << "train-volume" << std::setfill('0') << std::setw(4) << i_file << ".tif";
        truth_file_name << "train-labels" << std::setfill('0') << std::setw(4) << i_file << ".tif";

        namespace fs = boost::filesystem;
        const fs::path volume_file = m_dataset_path / volume_file_name.str();
        const fs::path truth_file = m_dataset_path / truth_file_name.str();

        cv::Mat volume = cv::imread(volume_file.string(), CV_LOAD_IMAGE_COLOR);
        cv::Mat truth = cv::imread(truth_file.string(), CV_LOAD_IMAGE_GRAYSCALE);

        // prepare sample image for better classification
        prepare_image(volume);

        // convert ground truth to grayscalce if needed
        if (truth.channels() != 1)
        {
            cv::cvtColor(truth, truth, CV_BGR2GRAY);
        }

        SampleExtractor sample_extractor(volume, truth, m_sample_size);
        for (size_t i_sample = 0; i_sample < m_num_samples_per_image; ++i_sample)
        {
            cv::Mat sample_image;
            bool foreground;
            // unpack the tuple
            std::tie(sample_image, foreground) = sample_extractor.extractRandomSample();
            #pragma omp critical
            {
                if (foreground)
                {
                    foreground_samples.push_back(sample_image);
                }
                else
                {
                    background_samples.push_back(sample_image);
                }
            }
        }
    }

    std::cout << "Extracted " << foreground_samples.size() << " and foreground samples and " <<
    background_samples.size() << " background samples!" << std::endl;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void Program::prepare_image(cv::Mat& image) const
{
    if (image.channels() != 1)
    {
        cvtColor(image, image, CV_BGR2GRAY);
    }
    equalizeHist(image, image);

    // TODO: this is only needed for the opencv implementation
    image.convertTo(image, CV_32FC1, 1 / 255.);
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
cv::Mat Program::classify_image(CvRTrees& forest, const cv::Mat& image) const
{
    cv::Mat border_image;
    cv::copyMakeBorder(image, border_image,
                       m_sample_size/2, m_sample_size/2, m_sample_size/2, m_sample_size/2,
                       cv::BORDER_REFLECT);

    cv::Mat classification_image;
    classification_image.create(image.rows, image.cols, CV_8UC1);

    for (int row = 0; row < classification_image.rows; ++row)
    {
        for (int col = 0; col < classification_image.cols; ++col)
        {
            cv::Rect patch_definition(col, row, m_sample_size, m_sample_size);
            cv::Mat patch(border_image, patch_definition);

            int classification = forest.predict(convert_to_row_vector<float>(patch));
            classification_image.at<uchar>(row, col) = 255 - (classification * 255);
        }
    }

    return classification_image;
}
//----------------------------------------------------------------------------------------------------------------------
