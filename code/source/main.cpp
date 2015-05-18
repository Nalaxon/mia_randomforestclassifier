#include <cstdlib>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv)
{
#ifdef _WIN32
  const std::string imagePath = "..\\..\\MIA_KU_2015_DataSet\\train-volume0001.tif";
#else
  const std::string imagePath = "../../MIA_KU_2015_DataSet/train-volume0001.tif";
#endif
 
  cv::Mat image;
  image = cv::imread(imagePath.c_str(), CV_LOAD_IMAGE_COLOR);   // Read the file

  if(!image.data ) // Check for invalid input
  {
    std::cout <<  "Could not open or find the image " << imagePath.c_str() << std::endl ;
    return EXIT_FAILURE;
  }

  cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Display window", image );                  // Show our image inside it.

  cv::waitKey(0);                                         // Wait for a keystroke in the window
  
  return EXIT_SUCCESS;
}
