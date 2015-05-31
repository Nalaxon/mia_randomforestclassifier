#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

/// class NodeFactory - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory {
  // Operations
public:
	create(Node<DATA_TYPE, LABEL_TYPE> **node, 
		const std::vectory<Sample<LABEL_TYPE, DATA_TYPE>> &S,
		std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S_left,
		std::vector<Sample<LABEL_TYPE, DATA_TYPE>> &S_right);
};

#endif
