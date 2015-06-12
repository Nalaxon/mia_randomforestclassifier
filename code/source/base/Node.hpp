#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"

#include <memory>

template <typename LABEL_TYPE, typename DATA_TYPE>
class Node
{
public:

	Node();

	//just try to define move constructor
	Node(Node&& other)
		: m_histogram()
		, m_threshold()
		, m_leftChild()
		, m_rightChild()
	{
		m_histogram = other.m_histogram;
		m_threshold = other.m_threshold;
		m_leftChild = std::move(other.m_leftChild);
		m_rightChild = std::move(other.m_rightChild);

	}

	Node& operator= (Node&& other)
	{
		if (this != other)
		{
			m_histogram = other.m_histogram;
			m_threshold = other.m_threshold;
			m_leftChild = std::move(other.m_leftChild);
			m_rightChild = std::move(other.m_rightChild);
		}
		return *this;
	}
  
  using SampleVector = std::vector<Sample<LABEL_TYPE, DATA_TYPE>>;

  enum class Direction
  {
    LEFT,
    RIGHT
  } ;

  const Histogram<LABEL_TYPE, DATA_TYPE>& predict(const DATA_TYPE& data) const;

  Node<LABEL_TYPE, DATA_TYPE>& getLeft() const
  {
    return *m_leftChild;
  }

  Node<LABEL_TYPE, DATA_TYPE>& getRight() const
  {
    return *m_rightChild;
  }

  void setLeft(std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> left)
  {
    m_leftChild = std::move(left);
  }

  void setRight(std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> right)
  {
    m_rightChild = std::move(right);
  }

  void split(const SampleVector& samples,
             SampleVector& samples_left,
             SampleVector& samples_right) const;

private:
  std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>> m_histogram;

protected:

  double m_threshold;
  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> m_leftChild;
  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> m_rightChild;

  virtual Direction split(const DATA_TYPE& data) const = 0;

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
