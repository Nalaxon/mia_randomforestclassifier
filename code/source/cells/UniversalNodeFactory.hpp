#ifndef UNIVERSALNODEFACTORY_HPP

#define UNIVERSALNODEFACTORY_HPP
#include "NodeFactory.hpp"
#include "Label.hpp"

#include <opencv2/opencv.hpp>

/// class UniversalNodeFactory - 
class UniversalNodeFactory : public NodeFactory<Label, cv::Mat> {
  // Operations
public:
	UniversalNodeFactory(std::initializer_list<NodeFactory<Label, cv::Mat>> factoryList);
};


#endif