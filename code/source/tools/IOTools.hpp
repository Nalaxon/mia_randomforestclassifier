#ifndef IOTOOLS_HPP
#define IOTOOLS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace fs = boost::filesystem;

namespace IOTools
{
  static std::function<bool(std::string)> createSuffixFileFilter(std::string suffix)
  {
    return [suffix](std::string filename) -> bool
    {
      return std::equal(suffix.rbegin(), suffix.rend(), filename.rbegin());
    };
  }

  static std::function<bool(std::string)> createPrefixFileFilter(std::string prefix)
  {
	  return [prefix](std::string filename) -> bool
	  {
		  return std::equal(prefix.begin(), prefix.end(), filename.begin());
	  };
  }

  static std::function<bool(std::string)> createRegexFileFilter(std::string regex)
  {
	  return [regex](std::string filename) -> bool
	  {
		  boost::cmatch match;
		  return boost::regex_match(filename.c_str(), match, boost::regex(regex));
	  };
  }

  static bool dir(const fs::path &path, std::vector<fs::path> &file_paths, std::function<bool(std::string)> filter = [](std::string x){return true; })
  {
	  file_paths.clear();
	  if (!fs::exists(path) || !fs::is_directory(path))
	  {
		  std::cerr << "[IOTools::dir] ERROR: Path '" + path.string() + "' leads not to a valid directory." << std::endl;
		  return false;
	  }

	  fs::directory_iterator it(path);
	  std::copy_if(it, fs::directory_iterator(), std::back_inserter(file_paths), [filter](fs::path p) -> bool {return filter(p.filename().string()); });
	  std::sort(file_paths.begin(), file_paths.end());

	  return true;
  }


  static void dir(const std::string &path, std::vector<cv::Mat> &image_list, int flags=1)
  {
    fs::path p(path);
    if (fs::exists(p) && fs::is_directory(p))
    {
      fs::directory_iterator it(p);
      std::vector<fs::path> path_list;
      std::copy(it, fs::directory_iterator(), std::back_inserter(path_list));
      std::sort (path_list.begin(), path_list.end());



      for (std::vector<fs::path>::iterator it = path_list.begin(); it != path_list.end(); it++)
      {
        std::string image_path = it->string();
        cv::Mat image = cv::imread(image_path, flags);
        if (image.data != NULL)
          image_list.push_back(image);
      }

    }
    else
      std::cerr << "[IOTools::dir] ERROR: Path '" + path + "' leads not to a valid directory." << std::endl;
  }

  template <class T> static void saveToFile(const std::vector<T> &vector, const std::string &filename)
  {
    std::ofstream os (filename, std::ios::binary);

    size_t size = vector.size();
    os.write((const char*)&size, sizeof(size));
    os.write((const char*)&vector[0], size * sizeof(T));
    os.close();
  }

  template <class T> static void loadFromFile(std::vector<T> &vector, const std::string &filename)
  {
    std::ifstream is(filename, std::ios::binary);

    size_t size;
    is.read((char*)&size, sizeof(size));
    vector.resize(size);

    is.read((char*)&vector[0], size * sizeof(T));
  }

};

#endif