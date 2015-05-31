#ifndef THRESHOLDNODEFACTORY_HPP
#define THRESHOLDNODEFACTORY_HPP

#include "NodeFactory.hpp"
#include "Label.hpp"

#include <opencv2\opencv.hpp>

/// class ThresholdNodeFactory - 
class ThresholdNodeFactory : public NodeFactory<Label, cv::Mat> {
};


#endif