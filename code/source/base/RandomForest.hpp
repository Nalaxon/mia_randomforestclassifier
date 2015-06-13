#ifndef RANDOMFORREST_HPP
#define RANDOMFORREST_HPP

#include "NodeFactory.hpp"
#include "RFParameter.hpp"
#include "RandomTree.hpp"
#include "types.hpp"

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomForest
{
public:
  using Params = RFParameter<LABEL_TYPE, DATA_TYPE>;

  using NodeFactoryPtr = NodeFactoryPtr_<LABEL_TYPE, DATA_TYPE>;

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  using RandomTreeVector = RandomTreeVector_<LABEL_TYPE, DATA_TYPE>;

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE>;

  using HistogramVector = HistogramVector_<LABEL_TYPE, DATA_TYPE>;

  //----------------------------------------------------------------------------
  RandomForest(Params params, NodeFactoryPtr nodeFactory)
  : m_params(params),
  m_nClasses(0),
  m_nodeFactory(nodeFactory)
  {
    for (unsigned int i = 0; i < m_params.getNumTrees(); ++i)
    {
      m_trees.emplace_back(m_params.getTreeParams(), nodeFactory);
    }
  }

  //----------------------------------------------------------------------------
  void train(const SampleVector& samples)
  {
//#pragma omp parallel for
    for (int i = 0; i < static_cast<int>(m_params.getNumTrees()); ++i)
    {
      // TODO: bagging
      m_trees[i].train(samples);
    }
  }

  //----------------------------------------------------------------------------
  LABEL_TYPE predict(const DATA_TYPE& data) const
  {
    return predict_prob(data).max();
  }

  //----------------------------------------------------------------------------
  HistogramType predict_prob(const DATA_TYPE& data) const
  {
    HistogramVector histograms;
#pragma omp parallel for
    for (unsigned int i = 0; i < m_params.getNumTrees(); ++i)
    {
      auto treeResult = m_trees[i].predict(data);
#pragma omp critical
      {
        histograms.push_back(treeResult);
      }
    }

    return m_params.getEnsembleFct()(histograms);
  }

private:

  // the forest parameters
  Params m_params;

  // the number of classes
  unsigned int m_nClasses;

  // the trees
  RandomTreeVector m_trees;

  // the node factory
  NodeFactoryPtr m_nodeFactory;
} ;

#endif
