#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"
#include "types.hpp"

#include <memory>
#include <boost/random/random_device.hpp>

// forward declaration for friend declaration
template<typename LABEL_TYPE, typename DATA_TYPE>
class UniversalNodeFactory;


template<typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory
{
  // this is necessary to get access to createRandomNode()...
  friend UniversalNodeFactory<LABEL_TYPE, DATA_TYPE>;
  
public:

  using NodeType = Node<LABEL_TYPE, DATA_TYPE>;

  using NodePtr = NodePtr_<LABEL_TYPE, DATA_TYPE>;

  using SampleVector = SampleVector_<LABEL_TYPE, DATA_TYPE>;

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE>;

  using HistogramPtr = HistogramPtr_<LABEL_TYPE, DATA_TYPE>;

  //----------------------------------------------------------------------------
  // Constructor
  //----------------------------------------------------------------------------
  NodeFactory() : m_rng()
  {
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
  NodePtr create(const SampleVector& samples, unsigned int num_tests)
  {
    HistogramPtr hist_samples(new HistogramType(samples));

    std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_left;
    std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_right;

    float max_info_gain = -0.1;
    NodePtr max_node;
    for (unsigned int i = 0; i < num_tests; ++i)
    {
      NodePtr node(createRandomNode());
      samples_left.clear();
      samples_right.clear();
      node->split(samples, samples_left, samples_right);

      HistogramType hist_left(samples_left);
      HistogramType hist_right(samples_right);

      float infoGain = hist_samples->informationGain({&hist_left, &hist_right});
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
  virtual NodePtr createRandomNode() = 0;
  
  // a random number generator
  boost::random::random_device m_rng;

} ;


#endif
