#ifndef NODE_HPP
#define NODE_HPP

#include "Histogram.hpp"

#include <memory>
#include <cgnslib_f.h>
#include <H5DSpublic.h>

/// class Node - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class Node {

private:
  Histogram<LABEL_TYPE, DATA_TYPE> m_histogram;

protected:

  double m_threshold;
  std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>> m_leftChild;
  std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>> m_rightChild;

    virtual Direction split(const DATA_TYPE& data) const = 0;

  void split(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples,
                std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_left,
                std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_right);

public:

    enum Direction {
        LEFT,
        RIGHT
    };

  const Histogram<LABEL_TYPE, DATA_TYPE>& predict (const DATA_TYPE &data) const;

    std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>>& getLeft() const
    {
        return m_leftChild;
    }

    std::unique_ptr<Node<DATA_TYPE, LABEL_TYPE>>& getRight() const
    {
        return m_rightChild;
    }

};

template <typename LABEL_TYPE, typename DATA_TYPE>
void Node::split(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples,
                 std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_left,
                 std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples_right)
{
    for (auto& sample : samples)
    {
        switch (split(sample.getData()))
        {
            case LEFT:
                samples_left.push_back(sample);
                break;
            case RIGHT:
                samples_right.push_back(sample);
                break;
        }
    }
}

template <typename LABEL_TYPE, typename DATA_TYPE>
const Histogram<LABEL_TYPE, DATA_TYPE>& Node::predict(const DATA_TYPE& data) const
{
    if (!m_leftChild  || !m_rightChild)
    {
        return m_histogram;
    }

    Direction direction = split(data);
    if (direction == LEFT)
    {
        return m_leftChild->predict(data);
    }
    else
    {
        return m_rightChild->predict(data);
    }
}

#endif
