#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>
#include <map>
#include <cmath>

#include "Sample.hpp"

template <typename LABEL_TYPE, typename DATA_TYPE>
class Histogram
{
public:
  using SampleVector = std::vector<Sample<LABEL_TYPE, DATA_TYPE>>;
  
  Histogram(const SampleVector &samples);

  unsigned int numElementsForLabel(LABEL_TYPE label) const;
  unsigned int numElementsTotal() const;

  float entropy() const;
  float informationGain(std::initializer_list<Histogram<LABEL_TYPE, DATA_TYPE>&> splits);

  LABEL_TYPE max() const
  {
    return m_max;
  }

private:
  
  unsigned int m_numTotal;
  LABEL_TYPE m_max;
  std::map<LABEL_TYPE, unsigned int> m_histMap;

} ;

template <typename LABEL_TYPE, typename DATA_TYPE>
Histogram<LABEL_TYPE, DATA_TYPE>::Histogram(const SampleVector &samples)
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

template <typename LABEL_TYPE, typename DATA_TYPE>
unsigned int Histogram<LABEL_TYPE, DATA_TYPE>::numElementsForLabel(LABEL_TYPE label) const
{
  if (m_histMap.count(label))
  {
    return m_histMap[label];
  } else
  {
    return 0;
  }
}

template <typename LABEL_TYPE, typename DATA_TYPE>
unsigned int Histogram<LABEL_TYPE, DATA_TYPE>::numElementsTotal() const
{
  return m_numTotal;
}

template <typename LABEL_TYPE, typename DATA_TYPE>
float Histogram<LABEL_TYPE, DATA_TYPE>::entropy() const
{
  float entropy = 0.;
  for (auto& entry : m_histMap)
  {
    float prob = ((float) entry.first) / m_numTotal;
    entropy -= prob * log2(prob);
  }
  return entropy;
}

template <typename LABEL_TYPE, typename DATA_TYPE>
float Histogram<LABEL_TYPE, DATA_TYPE>::informationGain(std::initializer_list<Histogram<LABEL_TYPE, DATA_TYPE>&> splits)
{
  float sum = 0.;
  for (auto& split : splits)
  {
    sum += ((float) split.m_numTotal) / m_numTotal * split.entropy();
  }
  return entropy() - sum;
}

#endif

