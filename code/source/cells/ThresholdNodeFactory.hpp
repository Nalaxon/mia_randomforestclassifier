#ifndef THRESHOLDNODEFACTORY_HPP
#define THRESHOLDNODEFACTORY_HPP

#include "NodeFactory.hpp"
#include "CellLabel.hpp"

#include <opencv2/opencv.hpp>

/// class ThresholdNodeFactory - 
class ThresholdNodeFactory : public NodeFactory<CellLabel, cv::Mat> {
};


#endif