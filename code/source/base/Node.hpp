#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"
#include "types.hpp"
#include "memory.hpp"
#include <string>

#include <iostream>
#include <sstream>

template <typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class Node
{
public:

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using NodePtr = NodePtr_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using HistogramPtr = HistogramPtr_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  Node()
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");
  }
  

  //just try to define move constructor
  Node(Node&& other)
  : m_histogram(std::move(other.m_histogram)),
  m_leftChild(std::move(other.m_leftChild)),
  m_rightChild(std::move(other.m_rightChild))
  {
    other.m_histogram = nullptr;
    other.m_leftChild = nullptr;
    other.m_rightChild = nullptr;
  }

  Node& operator= (Node&& other)
  {
    if (this != other)
    {
      m_histogram = std::move(other.m_histogram);
      m_leftChild = std::move(other.m_leftChild);
      m_rightChild = std::move(other.m_rightChild);
    }
    return *this;
  }

  enum class Direction
  {
    LEFT,
    RIGHT,
    NONE
  } ;

  //----------------------------------------------------------------------------

  void setLeft(NodePtr left)
  {
    m_leftChild = std::move(left);
  }

  //----------------------------------------------------------------------------

  void setRight(NodePtr right)
  {
    m_rightChild = std::move(right);
  }

  //----------------------------------------------------------------------------

  void setHistogram(HistogramPtr histogram)
  {
    m_histogram = std::move(histogram);
  }

  //----------------------------------------------------------------------------

  const HistogramType& predict(const DATA_TYPE& data, const ROI_TYPE roi) const
  {
    if (!m_leftChild || !m_rightChild)
    {
      return *m_histogram;
    }

    Direction direction = split(data, roi);
    if (direction == Direction::LEFT)
    {
      return m_leftChild->predict(data, roi);
    } 
    else if (direction == Direction::RIGHT)
    {
      return m_rightChild->predict(data, roi);
    }
    else
    {
      return *m_histogram;
    }
  }

  //----------------------------------------------------------------------------

  void split(const SampleVector& samples,
             SampleVector& samples_left,
             SampleVector& samples_right) const
  {
    for (unsigned int i = 0; i < samples.size(); ++i)//(const auto& sample : samples)
    {
        Direction direction = split(samples[i].get_data(), samples[i].get_roi());
      if (direction == Direction::LEFT)
      {
          samples_left.push_back(samples[i]);
      } else if (direction == Direction::RIGHT)
      {
          samples_right.push_back(samples[i]);
      }
    }
  }

  void setThreshold(const SampleVector& samples)
  {
      boost::random::random_device m_rng;
      boost::random::uniform_int_distribution<> m_index_dist(0, samples.size() - 1);
      int index = m_index_dist(m_rng);
      setThreshold(samples[index].get_data(), samples[index].get_roi());
  }
  
  void print_dot_format(std::ostream& stream, unsigned int& node_id) const
  {
    unsigned int this_node_id = node_id;
    if (m_leftChild)
    {
      stream << this_node_id << " -> " << ++node_id << ";" << std::endl;
      m_leftChild->print_dot_format(stream, node_id);
    }
    if (m_rightChild)
    {
      stream << this_node_id << " -> " << ++node_id << ";" << std::endl;
      m_rightChild->print_dot_format(stream, node_id);
    }
    
    std::ostringstream label;
    // print class name
    label << typeid(*this).name() << " : " << m_histogram->entropy() << std::endl;
    
    // print histogram data
    for (const auto& hist_entry : m_histogram->getData())
    {
      label << hist_entry.first.resolve_label_name() << ": " << static_cast<float>(hist_entry.second) / m_histogram->numElementsTotal() << " (" << hist_entry.second  << ")" << std::endl;
    }
    
    stream << this_node_id << R"([label=")" << label.str() << R"("];)" << std::endl;
  } 

private:

  // a pointer to the histogram in this node
  HistogramPtr m_histogram;

  // a pointer to the left child
  NodePtr m_leftChild;

  // a pointer to the right child
  NodePtr m_rightChild;

  double m_value[2];

protected:
	std::string class_name;

  //----------------------------------------------------------------------------
	virtual Direction split(const DATA_TYPE& data, const ROI_TYPE& roi) const = 0;

    virtual void setThreshold(const DATA_TYPE& data, const ROI_TYPE& roi) = 0;

} ;

#endif
