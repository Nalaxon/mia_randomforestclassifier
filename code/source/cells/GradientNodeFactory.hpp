#ifndef GRADIENTNODEFACTORY_HPP

#define GRADIENTNODEFACTORY_HPP
#include "NodeFactory.hpp"

#include "Label.hpp"
#include <opencv2\opencv.hpp>

/// class GradientNodeFactory - 
class GradientNodeFactory : public NodeFactory<Label, cv::Mat> {
};


#endif