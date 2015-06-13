#include "UniversalNodeFactory.hpp"

UniversalNodeFactory::UniversalNodeFactory(std::initializer_list<std::shared_ptr<NodeFactory>> factoryList)
: m_dist(0, factoryList.size() - 1),
m_factories(factoryList) {
}

UniversalNodeFactory::NodePtr UniversalNodeFactory::createRandomNode()
{
    unsigned int factory_idx = m_dist(m_rng);
    return m_factories[factory_idx]->createRandomNode();
}