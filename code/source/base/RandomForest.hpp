#ifndef RANDOMFORREST_HPP
#define RANDOMFORREST_HPP

#include "NodeFactory.hpp"
#include "RFParameter.hpp"
#include "RandomTree.hpp"
#include "types.hpp"
#include "Label.hpp"

#include <iostream>

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomForest
{
public:
  using Params = RFParameter<LABEL_TYPE, DATA_TYPE>;

  using NodeFactoryPtr = NodeFactoryPtr_<LABEL_TYPE, DATA_TYPE>;

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  using RandomTreeVector = RandomTreeVector_<LABEL_TYPE, DATA_TYPE>;

  using HistogramPtr = HistogramPtr_<LABEL_TYPE, DATA_TYPE>;

  using HistogramVector = HistogramVector_<LABEL_TYPE, DATA_TYPE>;

  using EnsembleFct = EnsembleFct_<LABEL_TYPE, DATA_TYPE>;

  //----------------------------------------------------------------------------

  RandomForest(Params params, NodeFactoryPtr nodeFactory)
  : m_params(params),
  m_nClasses(0),
  m_nodeFactory(nodeFactory)
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");
    for (unsigned int i = 0; i < m_params.m_num_trees; ++i)
    {
      m_trees.emplace_back(m_params.m_tree_params, nodeFactory);
    }
  }

  //----------------------------------------------------------------------------

  void train(const SampleVector& samples)
  {
#pragma omp parallel for
    for (int i = 0; i < static_cast<int> (m_params.m_num_trees); ++i)
    {
      // TODO: bagging
      m_trees[i].train(samples);
    }
  }

  //----------------------------------------------------------------------------

  LABEL_TYPE predict(const DATA_TYPE& data, EnsembleFct ensemble_fct) const
  {
    return predict_prob(data, ensemble_fct)->max();
  }


  //----------------------------------------------------------------------------

  HistogramPtr predict_prob(const DATA_TYPE& data, EnsembleFct ensemble_fct) const
  {
    HistogramVector histograms;
#pragma omp parallel for
    for (int i = 0; i < static_cast<int> (m_params.m_num_trees); ++i)
    {
      const auto& treeResult = m_trees[i].predict(data);
#pragma omp critical
      {
        histograms.push_back(*treeResult);
      }
    }

    return ensemble_fct(histograms);
  }

  //----------------------------------------------------------------------------

  float predict_prob(const DATA_TYPE& data, const LABEL_TYPE& label, EnsembleFct ensemble_fct) const
  {
    return predict_prob(data, ensemble_fct)->prob(label);
  }


  //----------------------------------------------------------------------------

  void print_dot_format(std::ostream& stream) const
  {
    for (const auto& tree : m_trees)
    {
      tree.print_dot_format(stream);
    }
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
