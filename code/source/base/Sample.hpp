#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <vector>

template<typename LABEL_TYPE, typename DATA_TYPE>
class Sample
{
public:

  const LABEL_TYPE& getLabel() const
  {
    return m_label;
  }

  void setLabel(const LABEL_TYPE& label)
  {
    m_label = label;
  }

  const DATA_TYPE& getData() const
  {
    return m_data;
  }

  void setData(const DATA_TYPE& data)
  {
    m_data = data;
  }

private:
  LABEL_TYPE m_label;
  DATA_TYPE m_data;
} ;

#endif
