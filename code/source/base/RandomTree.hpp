#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include "memory.hpp"
#include "RTParameter.hpp"
#include "LeafNode.hpp"
#include "NodeFactory.hpp"
#include "types.hpp"

#include <iostream>

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree
{
public:

  //----------------------------------------------------------------------------

  RandomTree(RTParameter params, std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>> nodeFactory)
    : m_params(params),
    m_nodeFactory(nodeFactory)
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");
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

  void train(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples)
  {
    m_root = trainInternal(samples, 0);
  }

  //----------------------------------------------------------------------------

  std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>> predict(const DATA_TYPE& data) const
  {
    if (!m_root)
    {
      throw std::runtime_error("The tree has not been trained yet!");
    }
    return std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>>(new Histogram<LABEL_TYPE, DATA_TYPE>(m_root->predict(data)));
  }

  void print_dot_format(std::ostream& stream) const
  {
    stream << "digraph randomtree {" << std::endl;
    if (m_root)
    {
      unsigned int node_counter = 0;
      m_root->print_dot_format(stream, node_counter);
    }
    stream << "}";
  }

private:

  //----------------------------------------------------------------------------

  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> trainInternal(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples, unsigned int depth)
  {
    std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>> hist_samples(new Histogram<LABEL_TYPE, DATA_TYPE>(samples));
    if (depth > m_params.m_max_depth || samples.size() < m_params.m_min_samples ||
      hist_samples->entropy() == 0)
    {
      std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> leaf(new LeafNode<LABEL_TYPE, DATA_TYPE>());
      leaf->setHistogram(std::move(hist_samples));
      return leaf;
    }

    std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_left, samples_right;

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
  std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>> m_nodeFactory;

  // the root node
  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> m_root;
};

#endif

