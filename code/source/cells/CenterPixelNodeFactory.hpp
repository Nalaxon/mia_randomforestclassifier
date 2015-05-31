#include "NodeFactory.hpp"
#include "Label.hpp"

#include <opencv2\opencv.hpp>

/// class CenterPixelNodeFactory - 
class CenterPixelNodeFactory : public NodeFactory<Label, cv::Mat> {
};

