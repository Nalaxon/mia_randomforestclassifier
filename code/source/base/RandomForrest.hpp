#ifndef RANDOMFORREST_HPP
#define RANDOMFORREST_HPP

/// class RandomForrest - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class RandomForrest {
  // Attributes
private:
  NodeFactory<LABEL_TYPE, DATA_TYPE> m_nodeFactory;
protected:
  RFParams m_params;
  size_t m_nClasses;
  std::vector<RandomTree<LABEL_TYPE, DATA_TYPE>> m_trees;
  // Operations
public:
  RandomForrest(RFParams params, NodeFactory<LABEL_TYPE, DATA_TYPE> nodeFactory);
  train(std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &samples);
  predict (std::vector<DATA_TYPE> &data, std::vector<LABEL_TYPE> &labels);
};

#endif