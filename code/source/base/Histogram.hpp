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
  m_hist_map()
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
      if (!m_hist_map.count(sample.getLabel()))
      {
        m_hist_map[sample.getLabel()] = 1;
      } else
      {
        m_hist_map[sample.getLabel()] += 1;
      }
      ++m_numTotal;

      if (m_hist_map[sample.getLabel()] > max_count)
      {
        max_count = m_hist_map[sample.getLabel()];
        m_max = sample.getLabel();
      }
    }
  }

  //----------------------------------------------------------------------------

  Histogram(Histogram&& other)
  : m_numTotal(other.m_numTotal),
  m_max(std::move(other.m_max)),
  m_hist_map(std::move(other.m_hist_map))
  {
    other.m_numTotal = 0;
  }

  Histogram& operator= (Histogram&& other)
  {
    if (this != other)
    {
      m_numTotal = other.m_numTotal;
      m_max = std::move(other.m_max);
      m_hist_map = std::move(other.m_hist_map);
    }
    return *this;
  }

  Histogram(const Histogram& other)
  : m_numTotal(other.m_numTotal),
  m_max(other.m_max),
  m_hist_map(other.m_hist_map)
  {
  }

  Histogram& operator=(Histogram other)
  {
    std::swap(*this, other);
    return *this;
  }
  
  //----------------------------------------------------------------------------

  Histogram operator+(const Histogram& other) const
  {
    Histogram hist_sum(*this);
    add(hist_sum, other);
    return hist_sum;
  }

  //----------------------------------------------------------------------------

  Histogram& operator+=(const Histogram& other)
  {
    add(*this, other);
    return *this;
  }

  //----------------------------------------------------------------------------
  
  unsigned int operator[](const LABEL_TYPE& label) const
  {
    return numElementsForLabel(label);
  }
  
  //----------------------------------------------------------------------------

  unsigned int numElementsForLabel(const LABEL_TYPE& label) const
  {
    if (m_hist_map.count(label))
    {
      return m_hist_map.at(label);
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
  
  float prob(const LABEL_TYPE& label) const
  {
    if (m_numTotal == 0)
    {
      return 0.f;
    }
    return static_cast<float>(numElementsForLabel(label)) / m_numTotal; 
  }

  //----------------------------------------------------------------------------

  float entropy() const
  {
    float entropy = 0.;
    for (const auto& entry : m_hist_map)
    {
      float prob = ((float)entry.second) / m_numTotal;
      entropy -= prob * log2(prob);
    }
    return entropy;
  }  
  
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------

  float informationGain(std::vector<Histogram*> splits) const
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
  
  const std::map<LABEL_TYPE, unsigned int>& getData() const
  {
    return m_hist_map;
  }

private:

  // total num of samples
  unsigned int m_numTotal;

  // label with the maximum count of samples
  LABEL_TYPE m_max;

  // num of sampels per label
  std::map<LABEL_TYPE, unsigned int> m_hist_map;

  static void add(Histogram& addTo, const Histogram& toAdd)
  {
    addTo.m_numTotal += toAdd.m_numTotal;
    for (const auto& other_entry : toAdd.m_hist_map)
    {
      addTo.m_hist_map[other_entry.first] += other_entry.second;
    }

    unsigned int max_value = 0;
    for (const auto& entry : addTo.m_hist_map)
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

