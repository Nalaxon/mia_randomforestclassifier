#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include "memory.hpp"
#include "RTParameter.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"
#include "types.hpp"

#include <iostream>

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree
{
public:

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  using NodeFactoryPtr = NodeFactoryPtr_<LABEL_TYPE, DATA_TYPE>;

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE>;
  
  using HistogramPtr = HistogramPtr_<LABEL_TYPE, DATA_TYPE>;

  using NodePtr = NodePtr_<LABEL_TYPE, DATA_TYPE>;

  //----------------------------------------------------------------------------

  RandomTree(RTParameter params, NodeFactoryPtr nodeFactory)
  : m_params(params),
  m_nodeFactory(nodeFactory)
  {
  }

  //just try to define move constructor

  RandomTree(RandomTree&& other)
  : m_params(std::move(other.m_params)),
  m_nodeFactory(other.m_nodeFactory),
  m_root(std::move(other.m_root))
  {
    other.m_nodeFactory = nullptr;
    other.m_root = nullptr;
    other.m_params = RTParameter();
  }

  RandomTree& operator= (RandomTree&& other)
  {
    if (this != other)
    {
      m_nodeFactory = std::move(other.m_nodeFactory);
      m_root = std::move(other.m_root);
      m_params = std::move(other.m_params);
    }

    return *this;
  }

  //----------------------------------------------------------------------------

  void train(const SampleVector& samples)
  {
    m_root = trainInternal(samples, 0);
  }

  //----------------------------------------------------------------------------

  HistogramPtr predict(const DATA_TYPE& data) const
  {
    if (!m_root)
    {
      throw std::runtime_error("The tree has not been trained yet!");
    }
    const auto& prediction = m_root->predict(data);
    return HistogramPtr(new HistogramType(prediction));
  }

  void printDotFormat(std::ostream& stream) const
  {
    stream << "digraph randomtree {" << std::endl;
    if (m_root)
    {
      unsigned int node_counter = 0;
      m_root->printDotFormat(stream, node_counter);
    }
    stream << "}";
  }

private:

  //----------------------------------------------------------------------------

  NodePtr trainInternal(const SampleVector& samples, unsigned int depth)
  {
    if (depth > m_params.m_max_depth || samples.size() < m_params.m_min_samples)
    {
      return std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE >> ();
    }

    SampleVector samples_left, samples_right;

    std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> node = m_nodeFactory->create(samples, m_params.m_num_tests_per_split);
    node->split(samples, samples_left, samples_right);
    node->setLeft(trainInternal(samples_left, depth + 1));
    node->setRight(trainInternal(samples_right, depth + 1));

    return node;
  }

protected:

  // the tree parameters
  RTParameter m_params;

  // the node factory
  NodeFactoryPtr m_nodeFactory;

  // the root node
  NodePtr m_root;
} ;

#endif

