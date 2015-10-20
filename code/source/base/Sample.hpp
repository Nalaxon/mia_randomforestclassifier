#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include "types.hpp"

#include <vector>

template<typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class Sample
{
public:

  //----------------------------------------------------------------------------
    Sample()
    {
        m_label = LABEL_TYPE();
        m_data = DATA_TYPE();
        m_roi = ROI_TYPE();

    }
  //----------------------------------------------------------------------------
	Sample(LABEL_TYPE m_label, DATA_TYPE m_data, ROI_TYPE m_roi) :
	  m_label(m_label), m_data(m_data), m_roi(m_roi)
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

  //----------------------------------------------------------------------------
  const ROI_TYPE& get_roi() const
  {
	  return m_roi;
  }

  //----------------------------------------------------------------------------
  void set_roi(const ROI_TYPE& roi)
  {
	  m_roi = roi;
  }

private:
  
  // the label
  LABEL_TYPE m_label;
  
  // the data
  DATA_TYPE m_data;

  //the region of interest
  ROI_TYPE m_roi;
} ;

#endif
