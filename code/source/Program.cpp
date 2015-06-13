//
// Created by chri on 30.05.15.
//

#include "Program.hpp"
#include "SampleExtractor.hpp"
#include "tools/utils.hpp"
#include "RandomForest.hpp"
#include "cells/PatchParameter.hpp"
#include "cells/Label.hpp"
#include "cells/CenterPixelNodeFactory.hpp"
#include "tools/ImageTools.hpp"
#include "cells/GradientNodeFactory.hpp"
#include "cells/TwoPixelNodeFactory.hpp"
#include "UniversalNodeFactory.hpp"

#include <iomanip>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/program_options.hpp>
#include <boost/random.hpp>

Histogram<Label, cv::Mat> sum_ensemble(const std::vector<Histogram<Label, cv::Mat>>&histograms) {
    Histogram<Label, cv::Mat> sum;
    for (const auto& hist : histograms) {
        sum += hist;
    }
    return sum;
}


//----------------------------------------------------------------------------------------------------------------------

int Program::run(int argc, char** argv) {
    if (!parse_command_line(argc, argv)) {
        return EXIT_FAILURE;
    }

    std::vector<Sample<Label, cv::Mat>> samples;
    extract_training_samples(samples);

    // the following is just tmp code using the OpenCv Random Forest!


    // select sum test samples
    unsigned long num_test_samples = 100;
    std::vector<Sample<Label, cv::Mat>> test_samples;
    for (unsigned int sample = 0; sample < num_test_samples; ++sample) {
        test_samples.push_back(samples.back());
        samples.pop_back();
    }

    auto ensmble_fct = sum_ensemble;
    RTParameter rt_params;
    rt_params.m_maxDepth = 15;
    rt_params.m_minSamples = 3;
    RFParameter<Label, cv::Mat> rf_params(
            true,
            rt_params,
            ensmble_fct, 10);
    PatchParameter patch_params;
    patch_params.patch_height = m_sample_size;
    patch_params.patch_width = m_sample_size;
    patch_params.max_value = 1.0;

    std::shared_ptr<CenterPixelNodeFactory> center_pixel_factory(new CenterPixelNodeFactory(patch_params));
    std::shared_ptr<GradientNodeFactory> gradient_factory(new GradientNodeFactory(patch_params));
    std::shared_ptr<TwoPixelNodeFactory> two_pixel_factory(new TwoPixelNodeFactory(patch_params));
    std::shared_ptr<UniversalNodeFactory<Label, cv::Mat>> factory(new UniversalNodeFactory<Label, cv::Mat>({
        center_pixel_factory,
        gradient_factory,
        two_pixel_factory
    }));

    RandomForest<Label, cv::Mat> forest(rf_params, factory);

    std::cout << "Start training..." << std::endl;
    forest.train(samples);

    std::cout << "Training done!" << std::endl;

    // test the forest

    //std::cout << num_correct << " correct classification of " << num_test_samples << std::endl;
    boost::filesystem::path test_volume_path;
    std::tie(test_volume_path, std::ignore) = resolve_data_path(1);
    cv::Mat test_image = cv::imread(test_volume_path.string(), CV_LOAD_IMAGE_COLOR);
    cv::Mat test_image_prepared = prepare_image(test_image);
    cv::namedWindow("inputwindow", CV_WINDOW_AUTOSIZE);
    cv::imshow("inputwindow", test_image);
    cv::Mat classification_image = classify_image(forest, test_image_prepared);

    cv::namedWindow("resultwindow", CV_WINDOW_AUTOSIZE);
    cv::imshow("resultwindow", classification_image);
    cv::waitKey(0);

    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

bool Program::parse_command_line(int argc, char** argv) {
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
    try {
        po::store(po::parse_command_line(argc, argv, option_desc),
                given_options);

        // handle help before notify!
        if (given_options.count("help")) {
            std::cout << option_desc << std::endl;
            return false;
        }

        // notify will validate the input
        po::notify(given_options);

    } catch (const po::error& e) {
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

void Program::extract_training_samples(std::vector<Sample<Label, cv::Mat>>&samples) const {
    //#pragma omp parallel for
    for (int i_file = 1; i_file <= 30; ++i_file) {
        std::ostringstream volume_file_name, truth_file_name;
        volume_file_name << "train-volume" << std::setfill('0') << std::setw(4) << i_file << ".tif";
        truth_file_name << "train-labels" << std::setfill('0') << std::setw(4) << i_file << ".tif";

        namespace fs = boost::filesystem;
        fs::path volume_file, truth_file;
        std::tie(volume_file, truth_file) = resolve_data_path(i_file);

        cv::Mat volume = cv::imread(volume_file.string(), CV_LOAD_IMAGE_COLOR);
        cv::Mat truth = cv::imread(truth_file.string(), CV_LOAD_IMAGE_GRAYSCALE);

        // prepare sample image for better classification
        cv::Mat data_image = prepare_image(volume);

        // convert ground truth to grayscalce if needed
        if (truth.channels() != 1) {
            cv::cvtColor(truth, truth, CV_BGR2GRAY);
        }

        SampleExtractor sample_extractor(data_image, truth, m_sample_size);
        for (size_t i_sample = 0; i_sample < m_num_samples_per_image; ++i_sample) {
            cv::Mat sample_image;
            bool foreground;
            // unpack the tuple
            std::tie(sample_image, foreground) = sample_extractor.extractRandomSample();
            Sample<Label, cv::Mat> sample(foreground ? Label::BORDER : Label::CELL, sample_image);
#pragma omp critical
            {
                samples.push_back(sample);
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

cv::Mat Program::prepare_image(const cv::Mat& image) const {
    cv::Mat prepared;
    prepared.create(image.rows, image.cols, CV_32FC3);
    auto channels = ImageTools::extract_channels<3>(prepared);

    if (image.channels() != 1) {
        cv::cvtColor(image, channels[0], CV_BGR2GRAY);
    }
    cv::equalizeHist(channels[0], channels[0]);


    // create gradient
    int scale = 1;
    int delta = 0;
    int ddepth = CV_32F;
    int blur_kernel_size = 15;
    cv::Mat blurred;
    cv::GaussianBlur(channels[0], blurred, cv::Size(blur_kernel_size, blur_kernel_size), 0.0);
    /// Gradient X
    cv::Mat grad;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Sobel(blurred, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    /// Gradient Y
    cv::Sobel(blurred, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    /// Total Gradient (approximate)
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    cv::integral(grad, channels[1], ddepth);

    // create integral image
    channels[0].convertTo(channels[0], CV_32FC1, 1 / 255.);
    cv::integral(channels[0], channels[2], ddepth);

    cv::copyMakeBorder(channels[0], channels[0], 0, 1, 0, 1, cv::BORDER_DEFAULT);

    cv::merge(channels, prepared);
    return prepared;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

cv::Mat Program::classify_image(const RandomForest<Label, cv::Mat>& forest, const cv::Mat& image) const {
    cv::Mat border_image;
    cv::copyMakeBorder(image, border_image,
            m_sample_size / 2, m_sample_size / 2, m_sample_size / 2, m_sample_size / 2,
            cv::BORDER_REFLECT);

    cv::Mat classification_image;
    classification_image.create(image.rows, image.cols, CV_8UC1);

    for (int row = 0; row < classification_image.rows; ++row) {
        for (int col = 0; col < classification_image.cols; ++col) {
            cv::Rect patch_definition(col, row, m_sample_size, m_sample_size);
            cv::Mat patch(border_image, patch_definition);

            Label classification = forest.predict(patch);
            uchar pixel_value = classification == Label::BORDER ? 0 : 255;
            classification_image.at<uchar>(row, col) = pixel_value;
        }
    }

    return classification_image;
}
//----------------------------------------------------------------------------------------------------------------------

Program::PathTuple Program::resolve_data_path(unsigned int id) const {
    std::ostringstream volume_file_name, truth_file_name;
    volume_file_name << "train-volume" << std::setfill('0') << std::setw(4) << id << ".tif";
    truth_file_name << "train-labels" << std::setfill('0') << std::setw(4) << id << ".tif";

    return std::make_tuple(m_dataset_path / volume_file_name.str(), m_dataset_path / truth_file_name.str());
}