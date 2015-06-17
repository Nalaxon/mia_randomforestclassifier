#ifndef BASENODE_HPP
#define	BASENODE_HPP

#include "base/Node.hpp"
#include "CellLabel.hpp"

#include <opencv/cv.h>

class BaseNode : public Node<CellLabel, cv::Mat>
{
} ;

#endif	/* BASENODE_HPP */

