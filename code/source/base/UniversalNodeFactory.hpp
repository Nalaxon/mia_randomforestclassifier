#ifndef UNIVERSALNODEFACTORY_HPP

#define UNIVERSALNODEFACTORY_HPP
#include "NodeFactory.hpp"

#include <vector>
#include <boost/random.hpp>

/// class UniversalNodeFactory - 

template<typename LABEL_TYPE, typename DATA_TYPE>
class UniversalNodeFactory : public NodeFactory<LABEL_TYPE, DATA_TYPE>
{
public:
  UniversalNodeFactory(std::initializer_list<std::shared_ptr<NodeFactory>> factoryList);

protected:
  virtual NodePtr createRandomNode();

private:
  boost::random::uniform_int_distribution<> m_dist;
  std::vector<std::shared_ptr<NodeFactory>> m_factories;
} ;


#endif