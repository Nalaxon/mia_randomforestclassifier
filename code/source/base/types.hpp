#ifndef TYPES_HPP
#define	TYPES_HPP

#include "memory.hpp"

#include <vector>
#include <functional>

template<typename LABEL_TYPE, typename DATA_TYPE>
class Sample;

template<typename LABEL_TYPE, typename DATA_TYPE>
class Histogram;

template<typename LABEL_TYPE, typename DATA_TYPE>
class RandomTree;

template<typename LABEL_TYPE, typename DATA_TYPE>
class Node;

template<typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory;

template<typename LABEL_TYPE, typename DATA_TYPE>
using SampleVector_ = std::vector<Sample<LABEL_TYPE, DATA_TYPE>>;

template<typename LABEL_TYPE, typename DATA_TYPE>
using HistogramVector_ = std::vector<Histogram<LABEL_TYPE, DATA_TYPE>>;

template<typename LABEL_TYPE, typename DATA_TYPE>
using RandomTreeVector_ = std::vector<RandomTree<LABEL_TYPE, DATA_TYPE>>;

template<typename LABEL_TYPE, typename DATA_TYPE>
using NodePtr_ = std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>;

template<typename LABEL_TYPE, typename DATA_TYPE>
using HistogramPtr_ = std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>>;

template<typename LABEL_TYPE, typename DATA_TYPE>
using NodeFactoryPtr_ = std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>>;

template<typename LABEL_TYPE, typename DATA_TYPE>
using EnsembleFct_ = std::function<HistogramPtr_<LABEL_TYPE, DATA_TYPE>(const HistogramVector_<LABEL_TYPE, DATA_TYPE>&)>;

#endif	/* TYPES_HPP */

