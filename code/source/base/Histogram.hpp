#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <initializer_list>

#include "Sample.hpp"
#include "types.hpp"

template <typename LABEL_TYPE, typename DATA_TYPE>
class Histogram
{
public:
  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  Histogram()
  : m_numTotal(0),
  m_max(),
  m_histMap()
  {

  }

  //----------------------------------------------------------------------------
  // Constructor
  //----------------------------------------------------------------------------

  Histogram(const SampleVector &samples)
  : m_numTotal(0)
  {
    unsigned int max_count = 0;
    for (const auto& sample : samples)
    {
      if (!m_histMap.count(sample.getLabel()))
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

  Histogram(Histogram&& other)
  : m_numTotal(other.m_numTotal),
  m_max(std::move(other.m_max)),
  m_histMap(std::move(other.m_histMap))
  {
    other.m_numTotal = 0;
  }

  Histogram& operator= (Histogram&& other)
  {
    if (this != other)
    {
      m_numTotal = other.m_numTotal;
      m_max = std::move(other.m_max);
      m_histMap = std::move(other.m_histMap);
    }
    return *this;
  }

  Histogram(const Histogram& other)
  : m_numTotal(other.m_numTotal),
  m_max(other.m_max),
  m_histMap(other.m_histMap)
  {
  }

  Histogram& operator=(Histogram other)
  {
    std::swap(*this, other);
    return *this;
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
      float prob = ((float) entry.second) / m_numTotal;
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

  Histogram operator+(const Histogram& other) const
  {
    Histogram hist_sum(*this);
    add(hist_sum, other);
    return hist_sum;
  }
  
  Histogram& operator+=(const Histogram& other)
  {
    add(*this, other);
    return *this;
  }

private:

  // total num of samples
  unsigned int m_numTotal;

  // label with the maximum count of samples
  LABEL_TYPE m_max;

  // num of sampels per label
  std::map<LABEL_TYPE, unsigned int> m_histMap;

  static void add(Histogram& addTo, const Histogram& toAdd)
  {
    addTo.m_numTotal += toAdd.m_numTotal;
    for (const auto& other_entry : toAdd.m_histMap)
    {
      addTo.m_histMap[other_entry.first] += other_entry.second;
    }

    unsigned int max_value = 0;
    for (const auto& entry : addTo.m_histMap)
    {
      if (entry.second > max_value)
      {
        max_value = entry.second;
        addTo.m_max = entry.first;
      }
    }
  }
  
} ;

#endif

