#ifndef TWOREGIONSNODE_HPP
#define	TWOREGIONSNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class TwoRegionsNode : public BaseNode
{
public:
  TwoRegionsNode(const cv::Rect& region1, const cv::Rect& region2, float threshold);
  
  virtual Direction split(const cv::Mat& mat) const;
  
private:
  
  cv::Rect m_region1, m_region2;
  float m_threshold;

} ;

#endif	/* TWOREGIONSNODE_HPP */
