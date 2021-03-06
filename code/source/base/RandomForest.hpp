#ifndef RANDOMFORREST_HPP
#define RANDOMFORREST_HPP

#include "NodeFactory.hpp"
#include "RFParameter.hpp"
#include "RandomTree.hpp"
#include "types.hpp"
#include "Label.hpp"

#include <algorithm>
#include <iostream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

template<typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class RandomForest
{
public:
  using Params = RFParameter<LABEL_TYPE, DATA_TYPE>;

  using NodeFactoryPtr = NodeFactoryPtr_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using RandomTreeVector = RandomTreeVector_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using HistogramPtr = HistogramPtr_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using HistogramVector = HistogramVector_<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

  using EnsembleFct = EnsembleFct_<LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

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
#if NDEBUG
#pragma omp parallel for
#endif
    for (int i = 0; i < static_cast<int> (m_params.m_num_trees); ++i)
    {
      const SampleVector* samples_to_use = &samples;
      std::unique_ptr<SampleVector> shuffled_samples;
      if (m_params.m_bagging)
      {
        shuffled_samples = std::make_unique<SampleVector>(samples);
        std::random_shuffle(shuffled_samples->begin(), shuffled_samples->end());
        // take only 50% of the shuffled samples
        shuffled_samples->erase(shuffled_samples->begin() + (shuffled_samples->size() / 2), shuffled_samples->end());
        samples_to_use = shuffled_samples.get();
      }
      m_trees[i].train(*samples_to_use);
    }
  }

  //----------------------------------------------------------------------------

  LABEL_TYPE predict(const DATA_TYPE& data, const ROI_TYPE& roi, EnsembleFct ensemble_fct) const
  {
    return predict_prob(data, roi, ensemble_fct)->max();
  }


  //----------------------------------------------------------------------------

  HistogramPtr predict_prob(const DATA_TYPE& data, const ROI_TYPE& roi, EnsembleFct ensemble_fct) const
  {
    HistogramVector histograms;
#if NDEBUG
#pragma omp parallel for
#endif
    for (int i = 0; i < static_cast<int> (m_params.m_num_trees); ++i)
    {
      const auto& treeResult = m_trees[i].predict(data, roi);
#if NDEBUG
#pragma omp critical
#endif
      {
        histograms.push_back(*treeResult);
      }
    }

    return ensemble_fct(histograms);
  }

  //----------------------------------------------------------------------------

  float predict_prob(const DATA_TYPE& data, const ROI_TYPE& roi, const LABEL_TYPE& label, EnsembleFct ensemble_fct) const
  {
    return predict_prob(data, roi, ensemble_fct)->prob(label);
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
