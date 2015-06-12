#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include <memory>

#include "RTParameter.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"
#include "types.hpp"

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree
{
public:

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  using NodeFactoryPtr = NodeFactoryPtr_<LABEL_TYPE, DATA_TYPE>;

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE>;

  using NodePtr = NodePtr_<LABEL_TYPE, DATA_TYPE>;

  //----------------------------------------------------------------------------
  RandomTree(RTParameter params, NodeFactoryPtr nodeFactory)
  : m_params(params),
  m_nodeFactory(nodeFactory)
  {
  }

  //----------------------------------------------------------------------------
  void train(const SampleVector& samples)
  {
    m_root = trainInternal(samples, 0);
  }

  //----------------------------------------------------------------------------
  const HistogramType& predict(const DATA_TYPE& data) const
  {
    return m_root->predict(data);
  }

private:

  //----------------------------------------------------------------------------
  NodePtr trainInternal(const SampleVector& samples, unsigned int depth)
  {
    if (depth > m_params.m_maxDepth || samples.size() < m_params.m_minSamples)
    {
      return std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE >> ();
    }

    SampleVector samples_left, samples_right;
    std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> node = m_nodeFactory->create(samples, 10); // TODO: num_sampless
    node->split(samples, samples_left, samples_right);
    node->setLeft(trainInternal(samples_left, depth + 1));
    node->setRight(trainInternal(samples_right, depth + 1));

    return node;
  }

protected:

  // the node factory
  NodeFactoryPtr m_nodeFactory;

  // the tree parameters
  RTParameter m_params;

  // the root node
  NodePtr m_root;
} ;

#endif

