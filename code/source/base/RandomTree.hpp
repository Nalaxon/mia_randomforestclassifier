#ifndef RANDOMTREE_HPP
#define RANDOMTREE_HPP

#include <memory>

#include "RTParameter.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"

/// class RandomTree - 
template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree
{
private:
    void trainInternal(std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>& node, const Sample<LABEL_TYPE, DATA_TYPE>::Vector& samples, unsigned int depth);
    // Attributes
protected:
    NodeFactory<LABEL_TYPE, DATA_TYPE> m_nodeFactory;
    RTParameter m_params;
    std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> m_root;
    // Operations
public:

    RandomTree(RTParameter params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory);

    void train(const Sample<LABEL_TYPE, DATA_TYPE>::Vector& samples);

    const Histogram<LABEL_TYPE, DATA_TYPE>& predict(const DATA_TYPE& data) const;
};

template<typename LABEL_TYPE, typename DATA_TYPE>
RandomTree::RandomTree(RTParameter params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory) : m_params(params),
                                                                                             m_nodeFactory(nodeFactory)
{
}

template<typename LABEL_TYPE, typename DATA_TYPE>
void RandomTree::train(const Sample<LABEL_TYPE, DATA_TYPE>::Vector& samples)
{
    trainInternal(m_root, samples, 0);
}

template<typename LABEL_TYPE, typename DATA_TYPE>
void RandomTree::trainInternal(std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>& node, const Sample<LABEL_TYPE, DATA_TYPE>::Vector& samples,
                               unsigned int depth)
{
    if (depth > m_params.m_maxDepth || samples.size() < m_params.m_minSamples)
    {
        return;
    }

    Sample<LABEL_TYPE, DATA_TYPE>::Vector samples_left, samples_right;
    m_nodeFactory.create(node, samples, samples_left, samples_right);
    trainInternal(node->getLeft(), samples_left, depth + 1);
    trainInternal(node->getRight(), samples_right, depth + 1);
}

template<typename LABEL_TYPE, typename DATA_TYPE>
const Histogram<LABEL_TYPE, DATA_TYPE>& RandomTree::predict(const DATA_TYPE& data) const
{
    return m_root->predict(data);
}

#endif

