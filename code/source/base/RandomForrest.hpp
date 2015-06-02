#ifndef RANDOMFORREST_HPP
#define RANDOMFORREST_HPP

#include "NodeFactory.hpp"
#include "RFParameter.hpp"
#include "RandomTree.hpp"

/// class RandomForrest -
template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomForrest
{
    // Attributes
private:
    NodeFactory<LABEL_TYPE, DATA_TYPE> m_nodeFactory;
protected:
    RFParameter m_params;
    size_t m_nClasses;
    std::vector<RandomTree<LABEL_TYPE, DATA_TYPE>> m_trees;
    // Operations
public:
    RandomForrest(RFParameter params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory);

    void train(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples);

    LABEL_TYPE predict(const DATA_TYPE& data) const;

    Histogram<LABEL_TYPE, DATA_TYPE> predict_prob(const DATA_TYPE& data) const;
};

template<typename LABEL_TYPE, typename DATA_TYPE>
RandomForrest::RandomForrest(RFParameter params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory) : m_params(params),
                                                                                                   m_nodeFactory(
                                                                                                           nodeFactory)
{
    for (unsigned int i = 0; i < m_params.getNumTrees(); ++i)
    {
        m_trees.push_back(RandomTree(m_params.getTreeParams(), nodeFactory));
    }
}

template<typename LABEL_TYPE, typename DATA_TYPE>
void RandomForrest::train(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples)
{
    #pragma omp parallel for
    for (int i = 0; i < m_params.getNumTrees(); ++i)
    {
        // TODO: bagging
        m_trees[i].train(samples);
    }
}

template<typename LABEL_TYPE, typename DATA_TYPE>
LABEL_TYPE RandomForrest::predict(const DATA_TYPE& data) const
{
    Histogram<LABEL_TYPE, DATA_TYPE> ensembleHistogram = predict_prob(data);
    return ensembleHistogram.max();
}

template<typename LABEL_TYPE, typename DATA_TYPE>
Histogram<LABEL_TYPE, DATA_TYPE> RandomForrest::predict_prob(const DATA_TYPE& data) const
{
    std::vector<Histogram<LABEL_TYPE, DATA_TYPE>> histograms;
    #pragma omp parallel for
    for (int i = 0; i < m_params.getNumTrees(); ++i)
    {
        const Histogram<LABEL_TYPE, DATA_TYPE>& treeResult = m_trees[i].predict(data);
        #pragma omp critical
        {
            histograms.push_back(treeResult);
        }
    }

    return m_params.getEnsembleFct()(histograms);
}

#endif
