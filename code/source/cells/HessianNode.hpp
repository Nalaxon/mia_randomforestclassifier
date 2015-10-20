#ifndef	HESSIANNODE_HPP
#define	HESSIANNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class HessianNode : public BaseNode
{
public:
	HessianNode(int test, cv::Point2d &pixel, double th_module, double th_trace,
		double th_determine, double th_orientation, double th_firsteigen,
		double th_secondeigen, double th_sgned, double th_gnsed, std::ostream* log_stream)
		:m_test(test), m_pixel(pixel), m_module(th_module), m_trace(th_trace),
		m_determine(th_determine), m_orientation(th_orientation), m_first(th_firsteigen),
		m_second(th_secondeigen), m_sgned(th_sgned), m_gnsed(th_gnsed), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
    float m_threshold, m_module, m_trace, m_determine, m_orientation, m_first, m_second, m_sgned, m_gnsed;
  int m_test;
  cv::Point2d m_pixel;
  std::ostream* m_log_stream;

  double calc_module(const std::vector<cv::Mat> &grads) const;
  double calc_trace(const std::vector<cv::Mat> &grads) const;
  double calc_determine(const std::vector<cv::Mat> &grads) const;
  double calc_firsteigenvalue(const std::vector<cv::Mat> &grads) const;
  double calc_secondeigenvalue(const std::vector<cv::Mat> &grads) const;
  double calc_orientation(const std::vector<cv::Mat> &grads) const;
  double calc_gnsed(const std::vector<cv::Mat> &grads) const;
  double calc_sgned(const std::vector<cv::Mat> &grads) const;
  void normalizeImage(cv::Mat &image) const;
} ;

#endif	/* HESSIANNODE_HPP */

