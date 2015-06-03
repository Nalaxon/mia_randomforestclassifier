#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include <memory>

#include "RTParameter.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree
{
public:

  using SampleVector = std::vector<Sample<LABEL_TYPE, DATA_TYPE>>;

  RandomTree(RTParameter params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory);

  void train(const SampleVector& samples);

  const Histogram<LABEL_TYPE, DATA_TYPE>& predict(const DATA_TYPE& data) const;

private:
  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>
  trainInternal(const SampleVector& samples,
                unsigned int depth);

protected:
  NodeFactory<LABEL_TYPE, DATA_TYPE> m_nodeFactory;
  RTParameter m_params;
  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> m_root;

} ;

template<typename LABEL_TYPE, typename DATA_TYPE>
RandomTree<LABEL_TYPE, DATA_TYPE>::RandomTree(RTParameter params,
                                              NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory)
: m_params(params),
m_nodeFactory(nodeFactory)
{
}

template<typename LABEL_TYPE, typename DATA_TYPE>
void RandomTree<LABEL_TYPE, DATA_TYPE>::train(const SampleVector& samples)
{
  m_root = trainInternal(m_root, samples, 0);
}

template<typename LABEL_TYPE, typename DATA_TYPE>
std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>
RandomTree<LABEL_TYPE, DATA_TYPE>::trainInternal(const SampleVector& samples,
                                                 unsigned int depth)
{
  if (depth > m_params.m_maxDepth || samples.size() < m_params.m_minSamples)
  {
    return;
  }

  SampleVector samples_left, samples_right;
  auto node = m_nodeFactory.create(samples, samples_left, samples_right);
  trainInternal(node->getLeft(), samples_left, depth + 1);
  trainInternal(node->getRight(), samples_right, depth + 1);
  return node;
}

template<typename LABEL_TYPE, typename DATA_TYPE>
const Histogram<LABEL_TYPE, DATA_TYPE>& RandomTree<LABEL_TYPE, DATA_TYPE>::predict(const DATA_TYPE& data) const
{
  return m_root->predict(data);
}

#endif

