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
#include "cells/TwoRegionNodeFactory.hpp"
#include "cells/SumNodeFactory.hpp"
#include "cells/TwoPixelGradientNodeFactory.hpp"
#include "cells/TwoRegionsGradientNodeFactory.hpp"
#include "cells/CannyEdgeNodeFactory.hpp"
#include "cells/Haar4WaveletNodeFactory.hpp"
#include "cells/HoGNodeFactory.hpp"
#include "cells/CannyEdgeMoment00NodeFactory.hpp"
#include "cells/GaborNodeFactory.hpp"
#include "cells/MinNodeFactory.hpp"
#include "cells/MaxNodeFactory.hpp"
#include "cells/SDNodeFactory.hpp"
#include "cells/VarianceNodeFactory.hpp"
#include "cells/MedianNodeFactory.hpp"
#include "cells/HessianNodeFactory.hpp"
#include "cells/MembraneProjectionNodeFactory.hpp"
#include "cells/BilateralNodeFactory.hpp"
#include "cells/GaussPyrNodeFactory.hpp"

#include <iomanip>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/program_options.hpp>
#include <boost/random.hpp>



std::unique_ptr<Histogram<CellLabel, std::vector<cv::Mat>, cv::Rect>>
sum_ensemble(const std::vector<Histogram<CellLabel, std::vector<cv::Mat>, cv::Rect>>&histograms) {
	auto sum = std::make_unique<Histogram<CellLabel, std::vector<cv::Mat>, cv::Rect >>();
    for (const auto& hist : histograms) {
        *sum += hist;
    }
    return sum;
}

Program::Program() : m_tree_output_stream(nullptr)
{
}

//----------------------------------------------------------------------------------------------------------------------

Program::~Program() {
    if (m_tree_output_stream != nullptr && m_tree_output_stream != &std::cout)
    {
        delete m_tree_output_stream;
    }

	if (m_log_stream != nullptr && m_log_stream != &std::cout)
	{
		delete m_log_stream;
	}
}

int Program::run(int argc, char** argv) {
    if (!parse_command_line(argc, argv)) {
        return EXIT_FAILURE;
    }

    if (m_use_xvalidation) {
        boost::random::random_device rng;
		boost::random::uniform_int_distribution<> dist_test_image(1, MAX_IMAGES);
        m_test_image_index = dist_test_image(rng);
    }

	std::vector<Sample<CellLabel, std::vector<cv::Mat>, cv::Rect>> pure_samples;

    extract_training_samples(pure_samples);

    // construct forest/tree parameters
	RFParameter<CellLabel, std::vector<cv::Mat>> rf_params;
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
	std::vector<std::shared_ptr<NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>>> factory_list;
    
    factory_list.push_back(std::make_shared<TwoPixelGradientNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<MaxNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<SDNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<VarianceNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<MedianNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<TwoRegionNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<TwoRegionsGradientNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<CenterPixelNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<GradientNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<TwoPixelNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<SumNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<GaussPyrNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<HaarWaveletNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<SURFFilterNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<Haar4WaveletNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<HoGNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<GaborNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<MinNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<CannyEdgeNodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<CannyEdgeMoment00NodeFactory>(patch_params));
    factory_list.push_back(std::make_shared<HessianNodeFactory>(patch_params));
	factory_list.push_back(std::make_shared<MembraneProjectionNodeFactory>(patch_params));
	//factory_list.push_back(std::make_shared<BilateralNodeFactory>(patch_params));
	

	std::shared_ptr<UniversalNodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect >>
		factory(new UniversalNodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect>(factory_list));

	m_log_stream = new std::ofstream(std::string("../bin/log.txt").c_str(), std::ofstream::out);
	factory->setLogStream(m_log_stream);

    RandomForest<CellLabel, std::vector<cv::Mat>, cv::Rect> forest(rf_params, factory);

    if (m_use_xvalidation) {
        double accuracy = xvalidation(forest, pure_samples, m_num_xvalidation_sets);
		std::cout << "training finished" << std::endl << "starting testing..." << std::endl;
			 
        // test the forest on image
        boost::filesystem::path test_volume_path, truth_file_path;
        std::tie(test_volume_path, truth_file_path) = resolve_data_path(m_test_image_index);
		//std::cout << "get image: " << test_volume_path.string() << std::endl;
		cv::Mat test_image = cv::imread(test_volume_path.string(), CV_LOAD_IMAGE_COLOR);
       // std::cout << "test: cols: " << test_image.cols << " rows: " << test_image.rows << " type: " << test_image.type() << std::endl;
        cv::Mat tmp = cv::imread(truth_file_path.string(), CV_LOAD_IMAGE_GRAYSCALE);
		
		if ((test_image.type() != CV_32FC1) && (test_image.channels() == 1)) {
			test_image.convertTo(test_image, CV_32FC1);
		}

		cv::Mat truth_image;
		if ((test_image.cols < tmp.cols) || (test_image.rows < tmp.rows))
			cv::pyrDown(tmp, truth_image, cv::Size(test_image.cols, test_image.rows));
		else if ((test_image.cols > tmp.cols) || (test_image.rows > tmp.rows))
			cv::pyrUp(tmp, truth_image, cv::Size(test_image.cols, test_image.rows));
		else
			truth_image = tmp;

        // convert ground truth to grayscalce if needed
        if (truth_image.channels() != 1) {
            cv::cvtColor(truth_image, truth_image, CV_BGR2GRAY);
        }

        truth_image.convertTo(truth_image, CV_32FC1, 1 / 255.);

        std::vector<cv::Mat> test_image_prepared = prepare_image(test_image);
        //cv::namedWindow("inputwindow", CV_WINDOW_AUTOSIZE);
        //cv::imshow("inputwindow", test_image); 
        cv::Mat prop_image = classify_image(forest, test_image_prepared);

        cv::Mat classify_image;
        cv::threshold(prop_image, classify_image, 0.5f, 1.0f, cv::THRESH_BINARY_INV);

        
        cv::Mat absdiff_image;
        cv::absdiff(classify_image, truth_image, absdiff_image);
        double num_false_pixels = static_cast<double>(cv::countNonZero(absdiff_image));
        double num_pixels = static_cast<double>(absdiff_image.rows * absdiff_image.cols);
        double image_accuracy = (num_false_pixels / num_pixels);
        std::cout << "accuracy of image is " << image_accuracy << std::endl;
        
        //cv::namedWindow("diff", CV_WINDOW_AUTOSIZE);
        //cv::imshow("diff", absdiff_image);

        //cv::namedWindow("groundTruth", CV_WINDOW_AUTOSIZE);
        //cv::imshow("groundTruth", truth_image);

        //cv::namedWindow("binClassify", CV_WINDOW_AUTOSIZE);
        //cv::imshow("binClassify", classify_image);

        cv::namedWindow("propwindow", CV_WINDOW_AUTOSIZE);
        cv::imshow("propwindow", prop_image);

        prop_image.convertTo(prop_image, CV_8UC1, 255.);
        absdiff_image.convertTo(absdiff_image, CV_8UC1, 255.);

        //cv::namedWindow("resultingWindow", CV_WINDOW_AUTOSIZE);
        //cv::imshow("resultingWindow", test_image);
        namespace chrono = std::chrono;
        
        std::ostringstream output_base;
        output_base.precision(3);
        output_base 
                << accuracy
                << "-"
                << chrono::system_clock::to_time_t(chrono::system_clock::now())
                << "-"
                << std::setfill('0') << std::setw(4) << m_test_image_index;

        std::string prop, overlay, diff, log;
        prop = output_base.str() + "-prop.tif";
        cv::imwrite((m_log_path / prop).string(), prop_image);
        overlay = output_base.str() + "-overlay.tif";
        cv::imwrite((m_log_path / overlay).string(), test_image);
        diff = output_base.str() + "-diff.tif";
        cv::imwrite((m_log_path / diff).string(), absdiff_image);
        log = output_base.str() + "-log.txt";

		//Watershed
		cv::Mat watershed = segment_image(classify_image, prop_image, "watershed");
		
        boost::filesystem::path logfile_path = m_log_path / log;
        std::ofstream logfile(logfile_path.string());
        if (logfile.is_open())
        {
            logfile.precision(3);
            logfile << "x-accuracy:,accuracy:,num_trees:,max_depth:,num_feature_tests:,num_samples:,test_image_index:"
                    << std::endl
                    << accuracy
                    << ","
                    << image_accuracy
                    << ","
                    << m_num_trees
                    << ","
                    << m_max_depth
                    << ","
                    << m_num_feature_tests
                    << ","
                    << m_num_samples_per_image
                    << ","
                    << std::setfill('0') << std::setw(4) << m_test_image_index;
            logfile.close();
        }
        else
        {
            std::cerr << "Can't open file " << logfile_path << std::endl;
        }

    } else {
        //train forest
        std::cout << "Start training... " << std::endl;
        auto start = std::chrono::system_clock::now();
        forest.train(pure_samples);
        auto end = std::chrono::system_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout << "Training done! Took " << elapsed_seconds << " seconds." << std::endl;

        if (m_tree_output_stream) {
            //std::cout << "Tree structure:" << std::endl;
            forest.print_dot_format(*m_tree_output_stream);
        }

		// test the forest
		boost::filesystem::path test_volume_path;
		std::tie(test_volume_path, std::ignore) = resolve_data_path(30);

        std::cout << "Evaluating image: " << test_volume_path << std::endl;
        
		cv::Mat test_image = cv::imread(test_volume_path.string(), CV_LOAD_IMAGE_COLOR);
		std::vector<cv::Mat> test_image_prepared = prepare_image(test_image);
		cv::Mat prob_image = classify_image(forest, test_image_prepared);
        cv::Mat classify_image;
        cv::threshold(prob_image, classify_image, 0.5f, 1.0f, cv::THRESH_BINARY);

        cv::namedWindow("inputwindow", CV_WINDOW_AUTOSIZE);
        cv::imshow("inputwindow", test_image);

        cv::namedWindow("probwindow", CV_WINDOW_AUTOSIZE);
        cv::imshow("probwindow", prob_image);

        cv::namedWindow("binClassify", CV_WINDOW_AUTOSIZE);
        cv::imshow("binClassify", classify_image);
		cv::waitKey(0);

		
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

            ("sample_size", po::value<unsigned int>()->default_value(32),
            "the size of the samples. (Each sample will be (sample_size x sample_size)")

            ("enable_bagging", po::bool_switch()->default_value(false),
            "enable bagging of the input samples (-> use only randomly drawn 50% of the samples for each tree)")

            ("num_trees", po::value<unsigned int>()->default_value(10),
            "the number of trees to train")

            ("max_depth", po::value<unsigned int>()->default_value(7),
            "the maximal depth of each tree")

            ("min_samples_per_node", po::value<unsigned int>()->default_value(10),
            "the minimum number of samples per node at which a node will be splitted again.")

            ("num_feature_tests", po::value<unsigned int>()->default_value(150),
            "the number of feature tests to try at each split at each split.")

            ("print_trees", po::value<std::string>(),
            "enable printing of the trees after training in dot format to the standard output.")

            ("use_xvalidation", po::bool_switch()->default_value(false),
            "Use cross validation.")
    
            ("num_xvalidations", po::value<unsigned int>()->default_value(10),
            "the number of validation sets for xvalidation.")

			("log", po::value<std::string>()->default_value("."),
			"define output folder to log performance and images, must not empty");

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

    } catch (const std::exception& e) {
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

    m_use_xvalidation = given_options["use_xvalidation"].as<bool>();
    
    m_num_xvalidation_sets = given_options["num_xvalidations"].as<unsigned int>();

	fs::path log_path(given_options["log"].as<std::string>());
	m_log_path = log_path.string();

    if (!given_options.count("print_trees")) {
        m_tree_output_stream = nullptr;
    } else {
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

void Program::extract_training_samples(std::vector<Sample<CellLabel, std::vector<cv::Mat>, cv::Rect>>&samples) const {
#if NDEBUG
#pragma omp parallel for
#endif
	for (int i_file = 1; i_file <= MAX_IMAGES; ++i_file) {

		if (m_use_xvalidation && (i_file == m_test_image_index)) {
			std::cout << "test image index: " << i_file << std::endl;
			continue;
		}
            

        namespace fs = boost::filesystem;
        fs::path volume_file, truth_file;
        std::tie(volume_file, truth_file) = resolve_data_path(i_file);

		cv::Mat volume = cv::imread(volume_file.string(), CV_LOAD_IMAGE_COLOR);
		cv::Mat tmp = cv::imread(truth_file.string(), CV_LOAD_IMAGE_GRAYSCALE);
		
		//make same size for gt and volume
		cv::Mat truth;
		if ((volume.cols < tmp.cols) || (volume.rows < tmp.rows))
			cv::pyrDown(tmp, truth, cv::Size(volume.cols, volume.rows));
		else if ((volume.cols > tmp.cols) || (volume.rows > tmp.rows))
			cv::pyrUp(tmp, truth, cv::Size(volume.cols, volume.rows));
		else
			truth = tmp;

		if ((volume.type() != CV_32FC1) && (volume.channels() == 1)) {
			volume.convertTo(volume, CV_32FC1);
		}

        // prepare sample image for better classification
        std::vector<cv::Mat> data_image = prepare_image(volume);

        // convert ground truth to grayscalce if needed
        if (truth.channels() != 1) {
            cv::cvtColor(truth, truth, CV_BGR2GRAY);
        }

        SampleExtractor sample_extractor(data_image, truth, m_sample_size);
        unsigned int num_foreground = 0;
        unsigned int num_background = 0;
        unsigned int samples_per_class = m_num_samples_per_image / 2;
        while (num_background < samples_per_class || num_foreground < samples_per_class) {
            std::vector<cv::Mat> sample_image;
            bool foreground;
            // unpack the tuple
			cv::Rect roi;
            std::tie(sample_image, foreground) = sample_extractor.extractRandomSample(roi);

            auto& effected_sample_counter = foreground ? num_foreground : num_background;
            if (effected_sample_counter < samples_per_class) {
                ++effected_sample_counter;
                Sample<CellLabel, std::vector<cv::Mat>, cv::Rect> sample(foreground ? CellLabel::Border() : CellLabel::Cell(), sample_image,
                    cv::Rect(0, 0, m_sample_size, m_sample_size));
#if NDEBUG
#pragma omp critical
#endif
                {
                    samples.push_back(sample);
                }
            }
        }
    }
    
    // shuffle samples to obtain a uniform distribution
    std::random_shuffle(samples.begin(), samples.end());
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

std::vector<cv::Mat> Program::prepare_image(const cv::Mat& image) const {
	int ddepth = CV_32F;
	std::vector<cv::Mat> prepared;
    //auto channels = ImageTools::extract_channels<4>(image);
	
	//Original grayscale image
	cv::Mat gray;
    if (image.channels() != 1) {
        cv::cvtColor(image, gray, CV_BGR2GRAY);

    }
	else {
		gray = image;
	}
    
	cv::equalizeHist(gray, gray);
    gray.convertTo(gray, CV_32FC1, 1 / 255.);
     
    // create integral image
    push_tuble(gray, prepared, ddepth);
	
    // create gaussian
    int scale = 1;
    int delta = 0;
    int blur_kernel_size = 15;
    cv::Mat blurred, blurred_f;
	cv::GaussianBlur(prepared[0], blurred, cv::Size(blur_kernel_size, blur_kernel_size), 1., 1.);
	blurred.convertTo(blurred_f, CV_32F, 1.0f / 255.);
    push_tuble(blurred, prepared, ddepth);

	// create Gradient
    /// Gradient X
	cv::Mat grad_abs;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Sobel(blurred, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    /// Gradient Y
    cv::Sobel(blurred, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    /// Total Gradient (approximate)
    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad_abs);
	//cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad_abs);
	cv::Mat grad_abs_f;
	grad_abs.convertTo(grad_abs_f, CV_32F, 1.0f / 255.0f);
    push_tuble(grad_abs, prepared, ddepth);

    return prepared;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

cv::Mat Program::classify_image(const RandomForest<CellLabel, std::vector<cv::Mat>, cv::Rect>& forest, const std::vector<cv::Mat>& images) const {
	std::vector<cv::Mat> border_images;
	for (int i = 0; i < images.size(); ++i)
	{
		cv::Mat border_image;
		cv::copyMakeBorder(images[i], border_image,
			m_sample_size / 2, m_sample_size / 2, m_sample_size / 2, m_sample_size / 2,
			cv::BORDER_REFLECT);
		border_images.push_back(border_image.clone());
	}
    cv::Mat classification_image;
    classification_image.create(images[0].rows, images[0].cols, CV_32FC1);

    for (int row = 0; row < classification_image.rows; ++row) {
        for (int col = 0; col < classification_image.cols; ++col) {
            cv::Rect patch_definition(col, row, m_sample_size, m_sample_size);

            float pixel_value = forest.predict_prob(border_images, patch_definition, CellLabel::Cell(), sum_ensemble);
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

double Program::xvalidation(RandomForest<CellLabel, std::vector<cv::Mat>, cv::Rect> &forest, const std::vector<Sample<CellLabel, std::vector<cv::Mat>, cv::Rect>>& pure_samples, unsigned int validations) {

	std::vector<Sample<CellLabel, std::vector<cv::Mat>, cv::Rect>> ground_truth, samples;
    auto offset = pure_samples.size() / validations;
    double accuracy = 0.0;
	
	for (unsigned int i = 0; i < validations; ++i) {
		ground_truth.clear();
		samples.clear();

		for (unsigned int j = i * offset; j < (i + 1) * offset; ++j)
		{
			samples.push_back(pure_samples[j]);
		}

		for (unsigned int e = (i + 1) * offset; e < pure_samples.size() - 1; ++e)
		{
			ground_truth.push_back(pure_samples[e]);
		}

		//train forest
		std::cout << "Start training... " << i + 1 << std::endl;
		auto start = std::chrono::system_clock::now();
		forest.train(samples);
		auto end = std::chrono::system_clock::now();
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		std::cout << "Training done! Took " << elapsed_seconds << " seconds." << std::endl;

		if (m_tree_output_stream) {
			//std::cout << "Tree structure:" << std::endl;
			forest.print_dot_format(*m_tree_output_stream);
		}

		// test the forest
		int sum_correct = 0;
		for (unsigned int i = 0; i < samples.size(); ++i) {
			if (forest.predict(samples[i].get_data(), samples[i].get_roi(), sum_ensemble) == samples[i].get_label())
			{
				++sum_correct;
			}
		}

		accuracy += (float)sum_correct / (float)samples.size();
		std::cout << sum_correct << " correct classification of " << samples.size() << std::endl;

	}

    accuracy /= validations;
    std::cout << "accuracy: " << accuracy << std::endl;

    return accuracy;

}

cv::Mat Program::segment_image(const cv::Mat& classify_image, const cv::Mat& prop_image, std::string options)
{
	if (options == "watershed")
		return watershed_image(classify_image, prop_image);
}

cv::Mat Program::watershed_image(const cv::Mat& classify_image, const cv::Mat& prop_image)
{
	//Watershed
	cv::Mat markers = cv::Mat::zeros(classify_image.rows, classify_image.cols, CV_8U);

	/*cv::threshold(prop_image, markers, 200, 1, CV_THRESH_BINARY);
	//markers.convertTo(markers, CV_32SC1);
	for (int x = 0; x < prop_image.cols; ++x)
	{
	for (int y = 0; y < prop_image.rows; ++y)
	{
	if (prop_image.at<char>(y, x) < 200 && prop_image.at<char>(y, x) > 100)
	markers.at<int>(y, x) = -1;
	}
	}*/
	cv::Mat classify_integral;
	cv::integral(classify_image, classify_integral);
	cv::Size marker_size(10, 10);
	for (int x = 0; x < classify_integral.cols - marker_size.width - 1; x += marker_size.width)
	{
		for (int y = 0; y < classify_integral.rows - marker_size.height - 1; y += marker_size.height)
		{
			cv::Rect patchPosition(cv::Point(x, y), marker_size);
			//cv::Mat patch(classify_integral, patchPosition);
			if (ImageTools::mean_from_integral<double>(classify_integral, patchPosition) < 0.1)
				//if (cv::countNonZero(patch) == 0)
			{
				cv::rectangle(markers, patchPosition, 128, CV_FILLED);
			}
		}
	}

	marker_size = cv::Size(5, 5);
	for (int x = 0; x < classify_image.cols - marker_size.width - 1; x += marker_size.width)
	{
		for (int y = 0; y < classify_image.rows - marker_size.height - 1; y += marker_size.height)
		{
			cv::Rect patchPosition(cv::Point(x, y), marker_size);
			//cv::Mat patch(classify_image, patchPosition);
			if (ImageTools::mean_from_integral<double>(classify_integral, patchPosition) > 0.9)
				//if (cv::countNonZero(patch) == marker_size.area())
			{
				cv::rectangle(markers, patchPosition, 255, CV_FILLED);
			}
		}
	}

	//cv::Mat markers_display;
	//markers.convertTo(markers_display, CV_8UC1, 255.);
	cv::namedWindow("markerwindow", CV_WINDOW_AUTOSIZE);
	cv::imshow("markerwindow", markers);
	cv::waitKey(0);

	cv::Mat classification_rgb;
	cv::cvtColor(prop_image, classification_rgb, CV_GRAY2BGR);
	//prop_image.convertTo(classification_rgb, CV_8UC1, 255);
	//cv::cvtColor(classification_rgb, classification_rgb, CV_GRAY2BGR);
	cv::Mat markers_wshd;
	markers.convertTo(markers_wshd, CV_32S);
	cv::watershed(classification_rgb, markers_wshd);
	cv::Mat watershed_display;
	markers_wshd.convertTo(watershed_display, CV_8UC1, 255.);
	cv::namedWindow("watershedwindow", CV_WINDOW_AUTOSIZE);
	cv::imshow("watershedwindow", watershed_display);
	cv::waitKey(0);

	return markers_wshd;
}

void Program::push_tuble(cv::Mat input, std::vector<cv::Mat>& prepared, int ddepth) const
{
	//push original image
    prepared.push_back(input);

    cv::Mat tmp;
	cv::integral(input, tmp, ddepth);

	// cut off first row and col from integral images
	cv::Rect roi(1, 1, prepared[0].cols, prepared[0].rows);
	prepared.push_back(cv::Mat(tmp, roi));
}
