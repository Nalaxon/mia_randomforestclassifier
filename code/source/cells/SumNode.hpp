#ifndef MEANNODE_HPP
#define MEANNODE_HPP

#include "BaseNode.hpp"

class SumNode : public BaseNode
{
public:
	SumNode(double threshhold, std::ostream* log_stream = nullptr)
		:m_threshold(threshhold), m_log_stream(log_stream){}

protected:
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;

  double m_threshold;
  std::ostream* m_log_stream;
};

#endif
