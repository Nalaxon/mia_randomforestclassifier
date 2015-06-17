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
  
  template<typename PixelType, unsigned int num_channels, unsigned int channel>
  static const PixelType& get_pixel(const cv::Mat& mat, unsigned int row, unsigned int col)
  {
    return mat.at<cv::Vec<PixelType, num_channels>>(row, col)[channel];
  }
  
  template<typename PixelType, unsigned int num_channels, unsigned int channel>
  static const PixelType& get_center_pixel(const cv::Mat& mat)
  {
    int center_col = mat.cols / 2;
    int center_row = mat.rows / 2;
    return get_pixel<PixelType, num_channels, channel>(mat, center_row, center_col);
  }
  
  template<unsigned int N>
  static std::vector<cv::Mat> extract_channels(const cv::Mat& channelImg)
  {
    std::vector<cv::Mat> channels(N);
    cv::split(channelImg, channels);
    return channels;
  }
}

#endif
