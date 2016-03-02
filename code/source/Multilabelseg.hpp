#ifndef MIA_2015_MULTILABELSEG_HPP
#define MIA_2015_MULTILABELSEG_HPP

#include <opencv2/core/core.hpp>
#include <boost/filesystem.hpp>

class Multilabelseg
{
  public:
   void watershed(const cv::Mat &prob_img, const boost::filesystem::path &out_file); 
  private:
};

#endif
