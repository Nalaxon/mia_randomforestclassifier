#ifndef THRESHOLDNODEFACTORY_HPP
#define THRESHOLDNODEFACTORY_HPP

#include "NodeFactory.hpp"
#include "CellLabel.hpp"

#include <opencv2/opencv.hpp>

/// class ThresholdNodeFactory - 
class ThresholdNodeFactory : public NodeFactory<CellLabel, std::vector<cv::Mat>, cv::Rect> {
};


#endif