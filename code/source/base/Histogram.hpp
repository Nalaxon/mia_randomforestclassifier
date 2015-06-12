#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>
#include <map>
#include <cmath>

#include "Sample.hpp"
#include "types.hpp"

template <typename LABEL_TYPE, typename DATA_TYPE>
class Histogram
{
public:
  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  //----------------------------------------------------------------------------
  // Constructor
  //----------------------------------------------------------------------------
  Histogram(const SampleVector &samples)
  {
    unsigned int max_count = 0;
    for (const auto& sample : samples)
    {
      if (m_histMap.count(sample.getLabel()))
      {
        m_histMap[sample.getLabel()] = 1;
      } else
      {
        m_histMap[sample.getLabel()] += 1;
      }
      ++m_numTotal;

      if (m_histMap[sample.getLabel()] > max_count)
      {
        max_count = m_histMap[sample.getLabel()];
        m_max = sample.getLabel();
      }
    }
  }

  //----------------------------------------------------------------------------
  unsigned int numElementsForLabel(const LABEL_TYPE& label) const
  {
    if (m_histMap.count(label))
    {
      return m_histMap[label];
    } else
    {
      return 0;
    }
  }

  //----------------------------------------------------------------------------
  unsigned int numElementsTotal() const
  {
    return m_numTotal;
  }

  //----------------------------------------------------------------------------
  float entropy() const
  {
    float entropy = 0.;
    for (const auto& entry : m_histMap)
    {
      float prob = ((float) entry.first) / m_numTotal;
      entropy -= prob * log2(prob);
    }
    return entropy;
  }

  //----------------------------------------------------------------------------
  float informationGain(std::initializer_list<Histogram*> splits) const
  {
    float sum = 0.;
    for (const auto& split : splits)
    {
      sum += ((float) split->m_numTotal) / m_numTotal * split->entropy();
    }
    return entropy() - sum;
  }

  //----------------------------------------------------------------------------
  const LABEL_TYPE& max() const
  {
    return m_max;
  }

private:

  // total num of samples
  unsigned int m_numTotal;
  
  // label with the maximum count of samples
  LABEL_TYPE m_max;
  
  // num of sampels per label
  std::map<LABEL_TYPE, unsigned int> m_histMap;

} ;

#endif

