#include "MembraneProjectionNode.hpp"
#include "Tools/ImageTools.hpp"

MembraneProjectionNode::Direction MembraneProjectionNode::split(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    if (calc_thresh(data, roi) < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}

double MembraneProjectionNode::calc_sum(const cv::Mat &collector) const
{
	return cv::sum(collector).val[0];
}

double MembraneProjectionNode::calc_mean(const cv::Mat &collector) const
{
	double result = 0.;

	return cv::mean(collector).val[0];
}

double MembraneProjectionNode::calc_stdv(const cv::Mat &collector) const
{
	cv::Mat mean, stdev;
	cv::meanStdDev(collector, mean, stdev);
	return stdev.at<double>(0, 0);
}

double MembraneProjectionNode::calc_median(const cv::Mat &collector) const
{
	double result = 0.;
	int mat_size = collector.rows;
	cv::Mat sorted;
	cv::sortIdx(collector, sorted, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);

	if (mat_size % 2 != 0)
		result = collector.at<float>(sorted.at<int>(mat_size / 2));
	else
		result = (collector.at<float>(sorted.at<int>(mat_size / 2 - 1)) +
		collector.at<float>(sorted.at<int>(mat_size / 2 + 1))) * 0.5;

	return result;
}

double MembraneProjectionNode::calc_max(const cv::Mat &collector) const
{
	double result = 0., minVal = 0.;
	cv::minMaxLoc(collector, &minVal, &result);

	return result;
}

double MembraneProjectionNode::calc_min(const cv::Mat &collector) const
{
	double result = 0.;
	cv::minMaxLoc(collector, &result);
	return result;
}

void MembraneProjectionNode::rotateImage(const cv::Mat &ref, cv::Mat &rotMat, const double angle) const
{
	cv::Point2f src_center(ref.cols / 2, ref.rows / 2);
	cv::Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	warpAffine(ref, rotMat, rot_mat, rotMat.size());
}

float MembraneProjectionNode::calc_thresh(const std::vector<cv::Mat>& data, const cv::Rect& roi) const
{
    float result = 0.;
    // create gradient
    int scale = 1;
    int delta = 0;
    int ddepth = CV_32F;
    int kernel_size = 19;
    int kernel_half = kernel_size / 2;

    cv::Mat grad = cv::Mat(data[2], roi);

    std::string test;

    //cv::Mat kernel = cv::Mat::zeros(kernel_size, kernel_size, CV_32F), filtered, collector;
    //for (int i = 0; i < kernel_size; ++i)
    //	kernel.at<float>(i, kernel_half) = 1;

    //cv::Mat ref = kernel.clone();

    //cv::copyMakeBorder(grad, grad, kernel_half, kernel_half, kernel_half, kernel_half, cv::BORDER_REPLICATE);
    cv::Mat collector, filtered;
    std::vector<cv::Mat> kernels = getRotationMatrices();
    //static int count = 0;
    //std::cout << ++count;
    for (int i = 0; i < 30; ++i)
    {
        cv::filter2D(grad, filtered, CV_32F, kernels[i]);
        collector.push_back(filtered.at<float>(m_pixel.x, m_pixel.y));
        //rotateImage(ref, kernel, angle);
    }

    switch (m_test){
    case 0:
    {
              test = "sum";
              result = calc_sum(collector);
              break;
    }
    case 1:
    {
              test = "mean";
              result = calc_mean(collector);
              break;
    }
    case 2:
    {
              test = "stdv";
              result = calc_stdv(collector);
              break;
    }
    case 3:
    {
              test = "median";
              result = calc_median(collector);
              break;
    }
    case 4:
    {
              test = "max";
              result = calc_max(collector);
              break;
    }
    case 5:
    {
              test = "min";
              result = calc_min(collector);
              break;
    }
    }

    if (m_log_stream != nullptr)
        *m_log_stream << "MembraneProjection;" << test << ";" << m_threshold << ";" << result << "; " << std::endl;

    return result;
}

void MembraneProjectionNode::setThreshold(const std::vector<cv::Mat>& data, const cv::Rect& roi)
{
    m_threshold = calc_thresh(data, roi);
}