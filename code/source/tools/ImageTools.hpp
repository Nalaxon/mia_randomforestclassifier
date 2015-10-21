#ifndef IMAGETOOLS_HPP
#define IMAGETOOLS_HPP

#include "Sample.hpp"
#include "cells/CellLabel.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>


namespace ImageTools
{
//  static void extractPatches(const cv::Mat &image, std::vector<cv::Mat> &patches, const cv::Size &patch_size, int xdiff=-1, int ydiff=-1, int borderType=-1, const cv::Scalar &value = cv::Scalar())
//  {
//    int w = patch_size.width;
//    int h = patch_size.height;
//    xdiff = (xdiff == -1) ? w : xdiff;
//    ydiff = (ydiff == -1) ? h : ydiff;
//
//    cv::Mat border_image;
//    if (borderType != -1)
//      cv::copyMakeBorder(image, border_image, h - 1, h - 1, w - 1, w - 1, borderType, value);
//    else
//      border_image = image;
//
//    for (int y = 0; y + h <= border_image.rows; y += (ydiff))
//      for (int x = 0; x + w <= border_image.cols; x += (xdiff))
//        patches.push_back(border_image(cv::Rect(x, y, w, h)));
//
//  }
//
//  static void extractPatches(const std::vector<cv::Mat> &images, std::vector<std::vector<cv::Mat>> &patches, const cv::Size &patch_size, int xdiff = -1, int ydiff = -1, int borderType = -1, const cv::Scalar &value = cv::Scalar())
//  {
//    for (auto &img : images)
//    {
//      std::vector<cv::Mat> cpatch;
//      extractPatches(img, cpatch, patch_size, xdiff, ydiff, borderType, value);
//      patches.push_back(cpatch);
//    }
//  }
  
  template<typename PixelType>
  static const PixelType& get_pixel(const cv::Mat& mat,
	  unsigned int row, unsigned int col)
  {
	  return mat.at<PixelType>(row, col);
  }
  
  template<typename PixelType>
  static const PixelType& get_center_pixel(const cv::Mat& mat)
  {
	  int center_col = mat.cols / 2;
	  int center_row = mat.rows / 2;
	  return get_pixel<PixelType>(mat, center_row, center_col);
  }
  
  template<typename PixelType>
  static PixelType sum_from_integral(const cv::Mat& mat, cv::Rect region)
  {
      const auto& left_up = get_pixel<PixelType>(mat, region.y, region.x);
      const auto& left_bottom = get_pixel<PixelType>(mat, region.y + region.height, region.x);
      const auto& right_up = get_pixel<PixelType>(mat, region.y, region.x + region.width);
      const auto& right_bottom = get_pixel<PixelType>(mat, region.y + region.height, region.x + region.width);
    return right_bottom - right_up - left_bottom + left_up;
  }
  
  template<typename PixelType>
  static PixelType mean_from_integral(const cv::Mat& mat, cv::Rect region)
  {
      return sum_from_integral<PixelType>(mat, region) / region.area();
  }
  
  static void adjust_line(int &start, int &end, const int &constraint)
  {
	  int tmp = 0;
	  if ((start + end) >= constraint)
	  {
		  tmp = constraint - start - 2;
		  if ((tmp <= 0))
		  {
			  start = (end / 2);
			  end = (end == 0) ? 1 : end / 2;
		  }
		  else
			  end = tmp;
	  }
	  else if (end == 0)
		  end = 1;

  }

  static void adjust_region(cv::Rect &region, const int const_width, const int const_height)
  {
	  adjust_line(region.x, region.width, const_width);
	  adjust_line(region.y, region.height, const_height);
  }

  static void normalizeImage(cv::Mat &image)
  {
      double minVal, maxVal;
      cv::minMaxLoc(image, &minVal, &maxVal);
      image.convertTo(image, image.type(), 1. / (maxVal - minVal), minVal / (maxVal - minVal));
  }
}

#endif
