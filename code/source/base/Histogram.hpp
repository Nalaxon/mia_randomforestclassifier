#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>
#include <map>
#include <cmath>

#include "Sample.hpp"

/// class Histogram -
template <typename LABEL_TYPE, typename DATA_TYPE>
class Histogram
{
  // Attributes
private:
    unsigned int m_numTotal;
    LABEL_TYPE m_max;
  std::map<LABEL_TYPE, unsigned int> m_histMap;
  // Operations
public:
	Histogram(std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &samples);

    unsigned int numElementsForLabel(LABEL_TYPE label) const;
    unsigned int numElementsTotal() const;

    float entropy() const;
    float informationGain(std::initializer_list<Histogram<LABEL_TYPE, DATA_TYPE>&> splits);

    LABEL_TYPE max() const
    {
        return m_max;
    }
};

template <typename LABEL_TYPE, typename DATA_TYPE>
Histogram<LABEL_TYPE, DATA_TYPE>::Histogram(std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &samples)
{
    unsigned int max_count = 0;
	for (auto& sample : samples)
    {
        if (m_histMap.count(sample.getLabel()))
        {
            m_histMap[sample.getLabel()] = 1;
        }
        else
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
unsigned int Histogram::numElementsForLabel(LABEL_TYPE label) const
{
    if (m_histMap.count(label))
    {
        return m_histMap[label];
    }
    else
    {
        return 0;
    }
}

template <typename LABEL_TYPE, typename DATA_TYPE>
unsigned int Histogram::numElementsTotal() const
{
    return m_numTotal;
}

template <typename LABEL_TYPE, typename DATA_TYPE>
float Histogram::entropy() const
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
float Histogram::informationGain(std::initializer_list<Histogram<LABEL_TYPE, DATA_TYPE>&> splits)
{
    float sum = 0.;
    for (auto& split : splits)
    {
        sum += ((float) split.m_numTotal) / m_numTotal * split.entropy();
    }
    return entropy() - sum;
}

#endif

