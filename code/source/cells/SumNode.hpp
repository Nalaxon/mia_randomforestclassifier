#ifndef MEANNODE_HPP
#define MEANNODE_HPP

#include "BaseNode.hpp"

class SumNode : public BaseNode
{
public:
	SumNode(double threshhold, std::ostream* log_stream = nullptr)
		:m_threshold(threshhold), m_log_stream(log_stream){}

protected:
  virtual Direction split(const cv::Mat& data) const;

private:
  double m_threshold;
  std::ostream* m_log_stream;
};

#endif
