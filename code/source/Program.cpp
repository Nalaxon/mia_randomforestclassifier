//
// Created by chri on 30.05.15.
//

#include "Program.hpp"
#include "SampleExtractor.hpp"
#include "tools/utils.hpp"
#include "base/RandomForest.hpp"
#include "cells/PatchParameter.hpp"
#include "cells/CellLabel.hpp"
#include "cells/CenterPixelNodeFactory.hpp"
#include "tools/ImageTools.hpp"
#include "cells/GradientNodeFactory.hpp"
#include "cells/TwoPixelNodeFactory.hpp"
#include "cells/HaarWaveletNodeFactory.hpp"
#include "cells/SURFFilterNodeFactory.hpp"
#include "UniversalNodeFactory.hpp"

#include <iomanip>
#include <chrono>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/program_options.hpp>
#include <boost/random.hpp>


std::unique_ptr<Histogram<CellLabel, cv::Mat>>
sum_ensemble(const std::vector<Histogram<CellLabel, cv::Mat>>&histograms) {
  auto sum = std::make_unique<Histogram<CellLabel, cv::Mat >>();
    for (const auto& hist : histograms) {
        *sum += hist;
    }
    return sum;
}


//----------------------------------------------------------------------------------------------------------------------

Program::~Program()
{
  if (m_tree_output_stream != nullptr && m_tree_output_stream != &std::cout)
    delete m_tree_output_stream;
}

int Program::run(int argc, char** argv) {
    if (!parse_command_line(argc, argv)) {
        return EXIT_FAILURE;
    }

	std::vector<Sample<CellLabel, cv::Mat>> pure_samples;
    extract_training_samples(pure_samples);

    // construct forest/tree parameters
    RFParameter<CellLabel, cv::Mat> rf_params;
    rf_params.m_bagging = m_bagging;
    rf_params.m_num_trees = m_num_trees;
    rf_params.m_tree_params.m_max_depth = m_max_depth;
    rf_params.m_tree_params.m_min_samples = m_min_samples_per_node;
    rf_params.m_tree_params.m_num_tests_per_split = m_num_feature_tests;

    // construct patch parameters for the factory
    PatchParameter patch_params;
    patch_params.patch_height = m_sample_size;
    patch_params.patch_width = m_sample_size;
    patch_params.max_value = 1.0;

    // construct the universalnodefactory
	std::vector<std::shared_ptr<NodeFactory<CellLabel, cv::Mat>>> factory_list;
	factory_list.push_back(std::make_shared<CenterPixelNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<GradientNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<TwoPixelNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<HaarWaveletNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<SURFFilterNodeFactory>(patch_params));

  std::shared_ptr<UniversalNodeFactory<CellLabel, cv::Mat >>
    factory(new UniversalNodeFactory<CellLabel, cv::Mat>(factory_list));

    RandomForest<CellLabel, cv::Mat> forest(rf_params, factory);

    if (m_print_trees) {
      *m_tree_output_stream << "Tree structure:" << std::endl;
      forest.printDotFormat(*m_tree_output_stream);
    }

	bool validate = true;
	if (validate == true)
	{
		//x-validation
		unsigned int validations = 10;
		xvalidation(forest, pure_samples, validations);
	}
	else
	{
		//train forest
		std::cout << "Start training... " << std::endl;
		auto start = std::chrono::system_clock::now();
		forest.train(pure_samples);
		auto end = std::chrono::system_clock::now();
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		std::cout << "Training done! Took " << elapsed_seconds << " seconds." << std::endl;

		if (m_print_trees) {
			std::cout << "Tree structure:" << std::endl;
			forest.printDotFormat(std::cout);
		}

		// test the forest
		boost::filesystem::path test_volume_path;
		std::tie(test_volume_path, std::ignore) = resolve_data_path(1);
		cv::Mat test_image = cv::imread(test_volume_path.string(), CV_LOAD_IMAGE_COLOR);
		cv::Mat test_image_prepared = prepare_image(test_image);
		cv::namedWindow("inputwindow", CV_WINDOW_AUTOSIZE);
		cv::imshow("inputwindow", test_image);
		cv::Mat classification_image = classify_image(forest, test_image_prepared);

		cv::namedWindow("resultwindow", CV_WINDOW_AUTOSIZE);
		cv::imshow("resultwindow", classification_image);
	}
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
            "the size of the samples. (Each sample will be (sample_size x sample_size)")

            ("enable_bagging", po::bool_switch()->default_value(true),
            "enable bagging of the input samples")

            ("num_trees", po::value<unsigned int>()->default_value(10),
            "the number of trees to train")

            ("max_depth", po::value<unsigned int>()->default_value(7),
            "the maximal depth of each tree")

            ("min_samples_per_node", po::value<unsigned int>()->default_value(10),
            "the minimum number of samples per node at which a node will be splitted again.")

            ("num_feature_tests", po::value<unsigned int>()->default_value(150),
            "the number of feature tests to try at each split at each split.")

            ("print_trees", po::value<std::string>(),
            "enable printing of the trees after training in dot format to the standard output.");

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

    m_bagging = given_options["enable_bagging"].as<bool>();

    m_num_trees = given_options["num_trees"].as<unsigned int>();

    m_max_depth = given_options["max_depth"].as<unsigned int>();

    m_min_samples_per_node = given_options["min_samples_per_node"].as<unsigned int>();

    m_num_feature_tests = given_options["num_feature_tests"].as<unsigned int>();


    if (!given_options.count("print_trees"))
    {
      m_tree_output_stream = nullptr;
    }
    else
    {
      std::string file_path = given_options["print_trees"].as<std::string>();
      if (file_path.empty())
        m_tree_output_stream = &std::cout;
      else
        m_tree_output_stream = new std::ofstream(file_path.c_str(), std::ofstream::out);
    }
  
    

    // handle further options here if needed

    return true;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

void Program::extract_training_samples(std::vector<Sample<CellLabel, cv::Mat>>&samples ) const {
#pragma omp parallel for
    for (int i_file = 1; i_file <= 30; ++i_file) {

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
        unsigned int num_foreground = 0;
        unsigned int num_background = 0;
        unsigned int samples_per_class = m_num_samples_per_image / 2;
        while (num_background < samples_per_class || num_foreground < samples_per_class) {
            cv::Mat sample_image;
            bool foreground;
            // unpack the tuple
            std::tie(sample_image, foreground) = sample_extractor.extractRandomSample();
         
            auto& effected_sample_counter = foreground ? num_foreground : num_background;
            if (effected_sample_counter < samples_per_class) {
                ++effected_sample_counter;
                Sample<CellLabel, cv::Mat> sample(foreground ? CellLabel::Border() : CellLabel::Cell(), sample_image);
#pragma omp critical
                {
                    samples.push_back(sample);
                }
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
    cv::Mat grad_f;
    grad.convertTo(grad_f, CV_32F, 1.0f/255.0f);
    cv::integral(grad_f, channels[1], ddepth);

    // create integral image
    channels[0].convertTo(channels[0], CV_32FC1, 1 / 255.);
    cv::integral(channels[0], channels[2], ddepth);

    cv::copyMakeBorder(channels[0], channels[0], 0, 1, 0, 1, cv::BORDER_DEFAULT);

    cv::merge(channels, prepared);
    return prepared;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

cv::Mat Program::classify_image(const RandomForest<CellLabel, cv::Mat>& forest, const cv::Mat& image) const {
    cv::Mat border_image;
    cv::copyMakeBorder(image, border_image,
            m_sample_size / 2, m_sample_size / 2, m_sample_size / 2, m_sample_size / 2,
            cv::BORDER_REFLECT);

    cv::Mat classification_image;
    classification_image.create(image.rows, image.cols, CV_32FC1);

#pragma omp parallel for
    for (int row = 0; row < classification_image.rows; ++row) {
        for (int col = 0; col < classification_image.cols; ++col) {
            cv::Rect patch_definition(col, row, m_sample_size, m_sample_size);
            cv::Mat patch(border_image, patch_definition);

            float pixel_value = forest.predict_prob(patch, CellLabel::Cell(), sum_ensemble);
            classification_image.at<float>(row, col) = pixel_value;
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

float Program::xvalidation(RandomForest<CellLabel, cv::Mat> &forest, const std::vector < Sample<CellLabel, cv::Mat>> &pure_samples, const unsigned int validations) {
	
	std::vector<Sample<CellLabel, cv::Mat>> ground_truth, samples;
	unsigned int offset = std::floor(pure_samples.size() / validations);
	float accuracy = 0.0f;
	for (unsigned int i = 0; i < validations; ++i) {
		ground_truth.clear();
		samples.clear();

		for (int j = i*offset; j < (i + 1)*offset; ++j)
			samples.push_back(pure_samples[j]);

		for (int e = (i + 1)*offset; e < pure_samples.size() - 1; ++e)
			ground_truth.push_back(pure_samples[e]);

		//train forest
		std::cout << "Start training... " << i + 1 << std::endl;
		auto start = std::chrono::system_clock::now();
		forest.train(samples);
		auto end = std::chrono::system_clock::now();
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		std::cout << "Training done! Took " << elapsed_seconds << " seconds." << std::endl;

		if (m_print_trees) {
			std::cout << "Tree structure:" << std::endl;
			forest.printDotFormat(std::cout);
		}

		// test the forest
		CellLabel test_label;
		cv::Mat test_image_prepared;
		cv::Mat image_labeld;
		int sum_correct = 0;
		for (int i = 0; i < samples.size(); ++i) {
			if (forest.predict(samples[i].getData(), sum_ensemble) == samples[i].getLabel())
				++sum_correct;
		}

		accuracy += (float)sum_correct / (float)samples.size();
		std::cout << sum_correct << " correct classification of " << samples.size() << std::endl;
	}
	std::cout << "accuracy: " << accuracy / validations << std::endl;

}