#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"
#include "types.hpp"

#include <memory>

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
    RIGHT
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
    } else
    {
      return m_rightChild->predict(data);
    }
  }

  //----------------------------------------------------------------------------

  void split(const SampleVector& samples,
             SampleVector& samples_left,
             SampleVector& samples_right) const
  {
    for (const auto& sample : samples)
    {
      Direction direction = split(sample.getData());
      if (direction == Direction::LEFT)
      {
        samples_left.push_back(sample);
      } else
      {
        samples_right.push_back(sample);
      }
    }
  }

private:

  // a pointer to the histogram in this node
  HistogramPtr m_histogram;

  // a pointer to the left child
  NodePtr m_leftChild;

  // a pointer to the right child
  NodePtr m_rightChild;

protected:

  //----------------------------------------------------------------------------
  virtual Direction split(const DATA_TYPE& data) const = 0;

} ;

#endif
