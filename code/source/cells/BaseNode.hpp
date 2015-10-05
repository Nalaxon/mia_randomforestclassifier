#ifndef BASENODE_HPP
#define	BASENODE_HPP

#include "base/Node.hpp"
#include "CellLabel.hpp"
#include <string>
#include <fstream>

#include <opencv/cv.h>

class BaseNode : public Node<CellLabel, std::vector<cv::Mat>, cv::Rect>
{

} ;

#endif	/* BASENODE_HPP */

