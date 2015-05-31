#ifndef TWOPIXELNODEFACTORY_HPP
#define TWOPIXELNODEFACTORY_HPP
#include "NodeFactory.hpp"

#include "Label.hpp"
#include <opencv2\opencv.hpp>


/// class TwoPixelNodeFactory - 
class TwoPixelNodeFactory : public NodeFactory<Label, cv::Mat> {
};


#endif