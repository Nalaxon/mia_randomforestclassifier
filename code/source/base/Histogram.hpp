#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>

/// class Histrogram - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class Histrogram {
  // Attributes
private:
  std::map<LABEL_TYPE,int> m_histMap;
  // Operations
public:
	Histrogram(std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S);
};

template <typename LABEL_TYPE, typename DATA_TYPE>
Histrogram<LABEL_TYPE, DATA_TYPE>::Histrogram(std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S)
{
	
}

#endif