#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"
#include "types.hpp"
#include "memory.hpp"

#include <boost/random/random_device.hpp>

// forward declaration for friend declaration
template<typename LABEL_TYPE, typename DATA_TYPE>
class UniversalNodeFactory;


template<typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory
{
  // this is necessary to get access to createRandomNode()...
  friend UniversalNodeFactory < LABEL_TYPE, DATA_TYPE > ;

public:

  //----------------------------------------------------------------------------
  // Constructor
  //----------------------------------------------------------------------------
  NodeFactory() : m_rng()
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");
  }

  NodeFactory(const NodeFactory& other)
    : m_rng()
  {
  }

  NodeFactory& operator=(const NodeFactory& other)
  {
    std::swap(*this, other);
    return *this;
  }

  //----------------------------------------------------------------------------
  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> create(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples, unsigned int num_tests)
  {
    std::unique_ptr<Histogram<LABEL_TYPE, DATA_TYPE>> hist_samples(new Histogram<LABEL_TYPE, DATA_TYPE>(samples));

    std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_left;
    std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_right;

    float max_info_gain = -0.1f;
    std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> max_node;
    for (unsigned int i = 0; i < num_tests; ++i)
    {
      std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> node(createRandomNode());
      samples_left.clear();
      samples_right.clear();
      node->split(samples, samples_left, samples_right);

      Histogram<LABEL_TYPE, DATA_TYPE> hist_left(samples_left);
      Histogram<LABEL_TYPE, DATA_TYPE> hist_right(samples_right);

      std::vector<Histogram<LABEL_TYPE, DATA_TYPE>*> hist;
      hist.push_back(&hist_left);
      hist.push_back(&hist_right);

      float infoGain = hist_samples->informationGain(hist);
      if (infoGain > max_info_gain)
      {
        max_info_gain = infoGain;
        max_node = std::move(node);
      }
    }

    max_node->setHistogram(std::move(hist_samples));
    return max_node;
  }

protected:

  //----------------------------------------------------------------------------
  virtual std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> createRandomNode() = 0;

  // a random number generator
  boost::random::random_device m_rng;

};


#endif
