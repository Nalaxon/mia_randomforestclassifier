#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"

#include <memory>

template <typename LABEL_TYPE, typename DATA_TYPE>
class Node
{
  public:

  enum class Direction
  {
    LEFT,
    RIGHT
  } ;
  
  const Histogram<LABEL_TYPE, DATA_TYPE>& predict(const DATA_TYPE& data) const;

  Node<DATA_TYPE, LABEL_TYPE>& getLeft() const
  {
    return *m_leftChild;
  }

  Node<DATA_TYPE, LABEL_TYPE>& getRight() const
  {
    return *m_rightChild;
  }
  
private:
  std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>> m_histogram;

protected:

  using SampleVector = std::vector<Sample<LABEL_TYPE, DATA_TYPE>>;
  
  double m_threshold;
  std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>> m_leftChild;
  std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>> m_rightChild;

  virtual Direction split(const DATA_TYPE& data) const = 0;

  void split(const SampleVector& samples,
             SampleVector& samples_left,
             SampleVector& samples_right) const;

} ;

template <typename LABEL_TYPE, typename DATA_TYPE>
void Node<LABEL_TYPE, DATA_TYPE>::split(const SampleVector& samples,
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

template <typename LABEL_TYPE, typename DATA_TYPE>
const Histogram<LABEL_TYPE, DATA_TYPE>& Node<LABEL_TYPE, DATA_TYPE>::predict(const DATA_TYPE& data) const
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

#endif
