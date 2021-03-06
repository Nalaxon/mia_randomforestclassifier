#ifndef UNIVERSALNODEFACTORY_HPP
#define UNIVERSALNODEFACTORY_HPP

#include "NodeFactory.hpp"
#include "memory.hpp"

#include <vector>
#include <initializer_list>
#include <boost/random.hpp>

template<typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class UniversalNodeFactory : public NodeFactory<LABEL_TYPE, DATA_TYPE, ROI_TYPE >
{
public:  
  
	using SuperType = NodeFactory<LABEL_TYPE, DATA_TYPE, ROI_TYPE>;

    using SampleType = Sample < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

  //----------------------------------------------------------------------------
  UniversalNodeFactory(std::vector<std::shared_ptr<SuperType>> factory_list)
  : m_dist(0, factory_list.size() - 1),
  m_factories(factory_list)
  {
  }

  const std::vector<std::shared_ptr<SuperType>>& get_factories() const
  { 
    return m_factories;
  }

  void setLogStream(std::ostream* stream)
  {
	  m_log_stream = stream;
  }

  std::ostream* getLogStream()
  {
	  return m_log_stream;
  }

protected:
  
  //----------------------------------------------------------------------------
	virtual typename SuperType::NodePtr createRandomNode(std::ostream* log_stream)
  {
    unsigned int factory_idx = m_dist(SuperType::m_rng);
	if (log_stream != nullptr)
		*log_stream << m_factories[factory_idx]->get_ClassName();
    return m_factories[factory_idx]->createRandomNode(m_log_stream);
  }

  virtual std::string get_ClassName() { return (typeid(*this)).name(); };

private:
  
  boost::random::uniform_int_distribution<> m_dist;
  std::vector<std::shared_ptr<SuperType>> m_factories;
  std::ostream* m_log_stream;
} ;


#endif