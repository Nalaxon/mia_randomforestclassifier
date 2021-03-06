#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include "memory.hpp"
#include "RTParameter.hpp"
#include "LeafNode.hpp"
#include "NodeFactory.hpp"
#include "types.hpp"

#include <iostream>

template<typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class RandomTree
{
public:

	using SampleVector = SampleVector_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

  using NodeFactoryPtr = NodeFactoryPtr_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE > ;

  using HistogramType = Histogram < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

  using HistogramPtr = HistogramPtr_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

  using NodePtr = NodePtr_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

  //----------------------------------------------------------------------------

  RandomTree(RTParameter params, NodeFactoryPtr nodeFactory)
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

  void train(const SampleVector& samples)
  {
    m_root = trainInternal(samples, 0);
  }

  //----------------------------------------------------------------------------

  HistogramPtr predict(const DATA_TYPE& data, const ROI_TYPE roi) const
  {
    if (!m_root)
    {
      throw std::runtime_error("The tree has not been trained yet!");
    }
    return HistogramPtr(new HistogramType(m_root->predict(data, roi)));
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

  NodePtr trainInternal(const SampleVector& samples, unsigned int depth)
  {
    HistogramPtr hist_samples = std::make_unique<HistogramType>(samples);
    if (depth > m_params.m_max_depth || samples.size() < m_params.m_min_samples ||
      hist_samples->entropy() == 0)
    {
      NodePtr leaf = std::make_unique<LeafNode<LABEL_TYPE, DATA_TYPE, ROI_TYPE>>();
      leaf->setHistogram(std::move(hist_samples));
      return leaf;
    }

    SampleVector samples_left, samples_right;

    NodePtr node = m_nodeFactory->create(samples, m_params.m_num_tests_per_split);
    
    node->split(samples, samples_left, samples_right);
	if (samples_left.size() == 0 || samples_right.size() == 0)
	{
		NodePtr leaf = std::make_unique<LeafNode<LABEL_TYPE, DATA_TYPE, ROI_TYPE>>();
		leaf->setHistogram(std::move(hist_samples));
		return leaf;
	}
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
};

#endif

