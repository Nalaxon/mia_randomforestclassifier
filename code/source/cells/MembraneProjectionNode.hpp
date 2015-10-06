#ifndef	MEMBRANEPROJECTIONNODE_HPP
#define	MEMBRANEPROJECTIONNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class MembraneProjectionNode : public BaseNode
{
public:
	MembraneProjectionNode(int test, cv::Point2f &pixel, double th_sum, double th_mean,
		double th_stdv, double th_median, double th_max,
		double th_min, std::ostream* log_stream)
		:m_test(test), m_pixel(pixel), m_sum(th_sum), m_mean(th_mean),
		m_stdv(th_stdv), m_median(th_median), m_max(th_max),
		m_min(th_min), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
private:
  
  float m_sum, m_mean, m_stdv, m_median, m_max, m_min;
  int m_test;
  cv::Point2f m_pixel;
  std::ostream* m_log_stream;

  double calc_sum(const cv::Mat &collector) const;
  double calc_mean(const cv::Mat &collector) const;
  double calc_stdv(const cv::Mat &collector) const;
  double calc_median(const cv::Mat &collector) const;
  double calc_max(const cv::Mat &collector) const;
  double calc_min(const cv::Mat &collector) const;
  
  void normalizeImage(cv::Mat &image) const;
  void rotateImage(const cv::Mat &ref, cv::Mat &rotMat, const double angle) const;

  std::vector<cv::Mat>& getRotationMatrices() const
  {
	  static std::vector<cv::Mat> rotationMatrices;
	  int kernel_size = 19;
	  int kernel_half = kernel_size / 2;

	  if (rotationMatrices.size() == 0)
	  {

		  cv::Mat ref = cv::Mat::zeros(kernel_size, kernel_size, CV_32F), filtered, collector;
		  for (int i = 0; i < kernel_size; ++i)
		  {
			  ref.at<float>(i, kernel_half) = 1;
		  }

		  cv::Mat kernel = ref.clone();
		  for (int angle = 6; angle <= 180; angle += 6)
		  {
			  rotationMatrices.push_back(kernel.clone());
			  rotateImage(ref, kernel, angle);
		  }
	  }

	  return rotationMatrices;
  }
} ;

#endif	/* MEMBRANEPROJECTIONNODE_HPP */

