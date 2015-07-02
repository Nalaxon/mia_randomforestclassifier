#ifndef UNIVERSALNODEFACTORY_HPP
#define UNIVERSALNODEFACTORY_HPP

#include "NodeFactory.hpp"
#include "memory.hpp"

#include <vector>
#include <boost/random.hpp>

template<typename LABEL_TYPE, typename DATA_TYPE>
class UniversalNodeFactory : public NodeFactory<LABEL_TYPE, DATA_TYPE>
{
public:  
  

  //----------------------------------------------------------------------------
  UniversalNodeFactory(std::vector<std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>>> factory_list)
  : m_dist(0, factory_list.size() - 1),
  m_factories(factory_list)
  {
  }

  const std::vector<std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>>>& get_factories() const
  { 
    return m_factories;
  }

protected:
  
  //----------------------------------------------------------------------------
  virtual std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> createRandomNode()
  {
    unsigned int factory_idx = m_dist(NodeFactory<LABEL_TYPE, DATA_TYPE>::m_rng);
    return m_factories[factory_idx]->createRandomNode();
  }

private:
  
  boost::random::uniform_int_distribution<> m_dist;
  std::vector<std::shared_ptr<NodeFactory<LABEL_TYPE, DATA_TYPE>>> m_factories;
} ;


#endif