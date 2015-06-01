#include "tools\IOTools.hpp"
#include "tools\ImageTools.hpp"

#include <cstdlib>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include <boost/filesystem.hpp>

//#define BOOST_ALL_DYN_LINK 

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    fs::path imagePath("../../data/MIA_KU_2015_DataSet/");
	
	std::function<bool(std::string)> trn_label_ff_filter = IOTools::createRegexFileFilter("^train-labels.*\\.tif$");
	std::function<bool(std::string)> trn_volume_ff_filter = IOTools::createRegexFileFilter("^train-volume.*\\.tif$");

	std::vector<fs::path> trn_label_file_paths, trn_volume_file_paths;
	IOTools::dir(imagePath, trn_label_file_paths, trn_label_ff_filter);
	IOTools::dir(imagePath, trn_volume_file_paths, trn_volume_ff_filter);

 	std::vector<cv::Mat> trn_label_imgs, trn_volume_imgs;
	for (auto it = trn_label_file_paths.begin(); it != trn_label_file_paths.end(); it++)
		trn_label_imgs.push_back(cv::imread((*it).string(), CV_LOAD_IMAGE_GRAYSCALE));

	for (auto it = trn_volume_file_paths.begin(); it != trn_volume_file_paths.end(); it++)
		trn_volume_imgs.push_back(cv::imread((*it).string(), CV_LOAD_IMAGE_GRAYSCALE));

  std::vector<cv::Mat> trn_label_patches, trn_volume_patches;
  ImageTools::extractPatches(trn_label_imgs[0], trn_label_patches, cv::Size(32, 32));
  ImageTools::extractPatches(trn_label_imgs[0], trn_volume_patches, cv::Size(32, 32));


  




  boost::random::random_device rng;
  boost::random::uniform_int_distribution<> uniform_dist(1, 6);
  std::cout << "Random number: " << uniform_dist(rng) << std::endl;

    cv::Mat image;
	image = cv::imread(imagePath.string(), CV_LOAD_IMAGE_COLOR);   // Read the file

    if (!image.data) // Check for invalid input
    {
		std::cout << "Could not open or find the image " << imagePath.string() << std::endl;
        return EXIT_FAILURE;
    }

    cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);// Create a window for display.
    cv::imshow("Display window", image);                  // Show our image inside it.

    cv::waitKey(0);                                         // Wait for a keystroke in the window

    return EXIT_SUCCESS;
}
