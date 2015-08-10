#include "HoGNode.hpp"
#include "tools/ImageTools.hpp"

HoGNode::Direction HoGNode::split(const cv::Mat& mat) const
{
	cv::HOGDescriptor hog(mat.size(), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 8);
	//cv::HOGDescriptor hog;
	std::vector<float> ders;
	std::vector<cv::Point>locs;
	std::vector<cv::Rect> locations;

	cv::Mat gray = ImageTools::extract_channels<4>(mat)[0];
	gray.convertTo(gray, CV_8UC1, 255.);

	//hog.detectMultiScale(gray, locations, m_threshold, cv::Size(8, 8));
	hog.compute(gray, ders, cv::Size(8, 8), cv::Size(0, 0), locs);
	

	cv::Mat hogFeat;
	hogFeat.create(ders.size(), 1, CV_32FC1);

	double distance = 0;
	for (int i = 0; i < ders.size(); ++i)
		distance += abs(ders.at(i));

	*m_log_stream << "HoGNode;" << distance << ";" << std::endl;

	if (distance < m_threshold)
    {
        return Direction::LEFT;
    }
    else
    {
        return Direction::RIGHT;
    }
}

