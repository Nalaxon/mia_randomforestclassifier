#ifndef MEANNODE_HPP
#define MEANNODE_HPP

#include "BaseNode.hpp"

class SumNode : public BaseNode
{
public:
  SumNode(double threshhold);

protected:
  virtual Direction split(const cv::Mat& data) const;

private:
  double m_threshold;
};

#endif
