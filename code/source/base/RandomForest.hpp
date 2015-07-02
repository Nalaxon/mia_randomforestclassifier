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

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomForest
{
public:



  //----------------------------------------------------------------------------

  RandomForest(RFParameter<LABEL_TYPE, DATA_TYPE> params, std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>> nodeFactory)
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

  void train(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples)
  {
#pragma omp parallel for
    for (int i = 0; i < static_cast<int> (m_params.m_num_trees); ++i)
    {
      const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>* samples_to_use = &samples;
      std::unique_ptr<std::vector<Sample<LABEL_TYPE, DATA_TYPE>>> shuffled_samples;
      if (m_params.m_bagging)
      {
        shuffled_samples = std::unique_ptr<std::vector<Sample<LABEL_TYPE, DATA_TYPE>>>(new std::vector<Sample<LABEL_TYPE, DATA_TYPE>>(samples));
        std::random_shuffle(shuffled_samples->begin(), shuffled_samples->end());
        // take only 50% of the shuffled samples
        shuffled_samples->erase(shuffled_samples->begin() + (shuffled_samples->size() / 2), shuffled_samples->end());
        samples_to_use = shuffled_samples.get();
      }
      m_trees[i].train(*samples_to_use);
    }
  }

  //----------------------------------------------------------------------------

  LABEL_TYPE predict(const DATA_TYPE& data, std::function<std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>>(const std::vector<Histogram<LABEL_TYPE, DATA_TYPE>>&)> ensemble_fct) const
  {
    return predict_prob(data, ensemble_fct)->max();
  }


  //----------------------------------------------------------------------------

  std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>> predict_prob(const DATA_TYPE& data, std::function<std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>>(const std::vector<Histogram<LABEL_TYPE, DATA_TYPE>>&)> ensemble_fct) const
  {
    std::vector<Histogram<LABEL_TYPE, DATA_TYPE>> histograms;
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

  float predict_prob(const DATA_TYPE& data, const LABEL_TYPE& label, std::function<std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>>(const std::vector<Histogram<LABEL_TYPE, DATA_TYPE>>&)> ensemble_fct) const
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
  RFParameter<LABEL_TYPE, DATA_TYPE> m_params;

  // the number of classes
  unsigned int m_nClasses;

  // the trees
  std::vector<RandomTree<LABEL_TYPE, DATA_TYPE>> m_trees;

  // the node factory
  std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>> m_nodeFactory;
} ;

#endif
