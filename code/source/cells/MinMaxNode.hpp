#ifndef	MINMAXNODE_HPP
#define	MINMAXNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class MinMaxNode : public BaseNode
{
public:
	MinMaxNode(double threshold, bool test, std::ostream* log_stream)
		:m_threshold(threshold), m_test(test), m_log_stream(log_stream) {}
  
  virtual Direction split(const cv::Mat& mat) const;

  const static bool TEST_MAX = true;
  const static bool TEST_MIN = false;
  
private:
  
  float m_threshold;
  bool  m_test;
  std::ostream* m_log_stream;
  

} ;

#endif	/* MINMAXNODE_HPP */

