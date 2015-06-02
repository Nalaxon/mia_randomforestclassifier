#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <vector>

/// class Sample - 
template<typename LABEL_TYPE, typename DATA_TYPE>
class Sample
{
    // Attributes
public:

    typedef std::vector<Sample<LABEL_TYPE, DATA_TYPE>> Vector;

    LABEL_TYPE getLabel() const
    {
        return m_label;
    }

    void setLabel(LABEL_TYPE label)
    {
        m_label = label;
    }

    DATA_TYPE getData() const
    {
        return m_data;
    }

    void setData(DATA_TYPE data)
    {
        m_data = data;
    }

private:
    LABEL_TYPE m_label;
    DATA_TYPE m_data;
};

#endif
