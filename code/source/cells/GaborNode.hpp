#ifndef	GABORNODE_HPP
#define	GABORNODE_HPP

#include "BaseNode.hpp"

#include <opencv2/opencv.hpp>

class GaborNode : public BaseNode
{
public:
	GaborNode(double threshold, double sig,
		double th, double lm, double gm, double ps, std::ostream* log_stream)
		:m_threshold(threshold), m_sig(sig), m_th(th),
		m_lm(lm), m_gm(gm), m_ps(ps), m_log_stream(log_stream) {}
  
	virtual Direction split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
    virtual void setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi);

private:

    float calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const;
  
	float m_threshold, m_sig, m_th, m_lm, m_gm, m_ps;
  std::ostream* m_log_stream;
  

} ;

#endif	/* GABORNODE_HPP */

