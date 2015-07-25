#ifndef	CANNYEDGENODE_HPP
#define	CANNYEDGENODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class CannyEdgeNode : public BaseNode
{
public:
	CannyEdgeNode(double canny_threshold, double threshold);
  
  virtual Direction split(const cv::Mat& mat) const;
  
private:
  
  float m_threshold, m_canny_threshold;

} ;

#endif	/* CANNYEDGENODE_HPP */

