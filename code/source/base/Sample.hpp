#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include "types.hpp"

#include <vector>

template<typename LABEL_TYPE, typename DATA_TYPE>
class Sample
{
public:

  //----------------------------------------------------------------------------
  Sample(LABEL_TYPE m_label, DATA_TYPE m_data) :
  m_label(m_label), m_data(m_data)
  {
  }

  //----------------------------------------------------------------------------
  const LABEL_TYPE& get_label() const
  {
    return m_label;
  }

  //----------------------------------------------------------------------------
  void set_label(const LABEL_TYPE& label)
  {
    m_label = label;
  }

  //----------------------------------------------------------------------------
  const DATA_TYPE& get_data() const
  {
    return m_data;
  }

  //----------------------------------------------------------------------------
  void set_data(const DATA_TYPE& data)
  {
    m_data = data;
  }

private:
  
  // the label
  LABEL_TYPE m_label;
  
  // the data
  DATA_TYPE m_data;
} ;

#endif
