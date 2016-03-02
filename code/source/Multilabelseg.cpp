#include "Multilabelseg.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

void Multilabelseg::watershed(const cv::Mat &prob_img, const boost::filesystem::path &out_file)
{
  cv::Mat binary_classified_image, bin;
  cv::threshold(prob_img, bin, 0.5, 1.0, cv::THRESH_BINARY);
//  cv::morphologyEx(bin, bin, 1, 
//                   cv::getStructuringElement(0, cv::Size(5, 5), cv::Point(3, 3)));
  cv::Mat markers;
  bin.convertTo(markers, CV_32SC1, 255);
  bin.convertTo(binary_classified_image, CV_8UC1, 255);
  std::vector<cv::Vec3b> colorTab;
  unsigned char r = 20, g = 50, b = 70;
  
  //unsure cell
  cv::Size marker_size = cv::Size(10, 10);
  for (int x = 0; x < binary_classified_image.cols - marker_size.width-1; x += marker_size.width)
  {
      for (int y = 0; y < binary_classified_image.rows - marker_size.height-1; y += marker_size.height)
      {
          cv::Rect patchPosition(cv::Point(x, y), marker_size);
          cv::Mat patch(binary_classified_image, patchPosition);
          if (cv::countNonZero(patch) == 0)
          {
              //colorTab.push_back(cv::Vec3b((b*x) % 255, (g*y) % 255, (r*x+y) % 255));
              //cv::rectangle(markers, patchPosition, colorTab.size(), CV_FILLED);
              cv::rectangle(markers, patchPosition, 255, CV_FILLED);
          }
      }
  }

   //sure cell (foreground)
   marker_size = cv::Size(10, 10);
   for (int x = 0; x < binary_classified_image.cols - marker_size.width-1; x += marker_size.width)
    {
        for (int y = 0; y < binary_classified_image.rows - marker_size.height-1; y += marker_size.height)
        {
            cv::Rect patchPosition(cv::Point(x, y), marker_size);
            cv::Mat patch(binary_classified_image, patchPosition);
            if (cv::countNonZero(patch) == marker_size.area())
            {
                cv::rectangle(markers, patchPosition, 128, CV_FILLED);
                //colorTab.push_back(cv::Vec3b((b*x) % 255, (g*y) % 255, (r*x+y) % 255));
                //cv::rectangle(markers, patchPosition, colorTab.size(), CV_FILLED);
            }
        }
    }
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat markerMask(prob_img.size(), CV_32SC1);
    markerMask = cv::Scalar::all(0);
    cv::findContours(markerMask, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

    /*if (!contours.empty())
    {
      int compCount;
      markers = cv::Scalar::all(0);
      int idx = 0;
      for(;idx >= 0; idx = hierarchy[idx][0], compCount++)
         cv::drawContours(markers, contours, idx, cv::Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);
      for(int i =0; i < compCount; ++i)
        colorTab.push_back(cv::Vec3b((b*idx) % 255, (g*idx) %255, (r*idx)%255));
    }*/
    
    //Write probability map
    cv::Mat prob_display;
    prob_img.convertTo(prob_display, CV_8UC1, 255);
    boost::filesystem::path prob_file(out_file);
    prob_file += "_prob.png";
    cv::imwrite(prob_file.string(), prob_display);

    //Write Markers
    cv::Mat markers_display;
    markers.convertTo(markers_display, CV_8UC1, 255);    
    boost::filesystem::path marker_file(out_file);
    marker_file += "_markers.png";
    cv::imwrite(marker_file.string(), markers_display);
   
    //Write threshold segmentation
    boost::filesystem::path binary_file(out_file);
    binary_file += "_threshold.png";
    cv::Mat binary_classified_image_display;
    binary_classified_image.convertTo(binary_classified_image_display, CV_8UC1, 255);
    cv::imwrite(binary_file.string(), binary_classified_image);

    //Watershed segmentation
    cv::Mat classification_rgb;
    prob_img.convertTo(classification_rgb, CV_8UC1, 255);
    cv::cvtColor(classification_rgb, classification_rgb, CV_GRAY2BGR);
    cv::watershed(classification_rgb, markers);

    cv::Mat wshed(markers.size(), CV_8UC3);
    // paint the watershed image
    /*for(int i = 0; i < markers.rows; ++i )
       for(int j = 0; j < markers.cols; ++j )
       {
         int index = markers.at<int>(i,j);
         if( index == -1 )
            wshed.at<cv::Vec3b>(i,j) = cv::Vec3b(255,255,255);
         else if( index <= 0 || index >  colorTab.size() )
            wshed.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
         else
            wshed.at<cv::Vec3b>(i,j) = colorTab[index - 1];
       }*/
    markers.convertTo(wshed, CV_8UC1, 255);
    boost::filesystem::path segmentation_file(out_file);
    segmentation_file += "_watershed.png";
    cv::imwrite(segmentation_file.string(), wshed);
    
    /*cv::namedWindow("watershedwindow", CV_WINDOW_AUTOSIZE);
    cv::imshow("watershedwindow", watershed_display);
    cv::waitKey(0);

    cv::namedWindow("markerwindow", CV_WINDOW_AUTOSIZE);
    cv::imshow("markerwindow", markers_display);
    cv::waitKey(0);

    cv::namedWindow("binClassify", CV_WINDOW_AUTOSIZE);
    cv::imshow("binClassify", binary_classified_image);
    cv::waitKey(0);*/
}
