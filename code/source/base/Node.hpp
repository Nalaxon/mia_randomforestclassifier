#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"
#include "types.hpp"
#include "memory.hpp"
#include <string>

#include <iostream>
#include <sstream>

template <typename LABEL_TYPE, typename DATA_TYPE>
class Node
{
public:

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE>;

  using NodePtr = NodePtr_<LABEL_TYPE, DATA_TYPE>;

  using HistogramPtr = HistogramPtr_<LABEL_TYPE, DATA_TYPE>;

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

  const HistogramType& predict(const DATA_TYPE& data) const
  {
    if (!m_leftChild || !m_rightChild)
    {
      return *m_histogram;
    }

    Direction direction = split(data);
    if (direction == Direction::LEFT)
    {
      return m_leftChild->predict(data);
    } 
    else if (direction == Direction::RIGHT)
    {
      return m_rightChild->predict(data);
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
    for (const auto& sample : samples)
    {
      Direction direction = split(sample.get_data());
      if (direction == Direction::LEFT)
      {
        samples_left.push_back(sample);
      } else if (direction == Direction::RIGHT)
      {
        samples_right.push_back(sample);
      }
    }
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
    label << typeid(*this).name() << std::endl;
    
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
  virtual Direction split(const DATA_TYPE& data) const = 0;

} ;

#endif
