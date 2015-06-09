#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"

#include <memory>
#include <boost/random/random_device.hpp>

template<typename LABEL_TYPE, typename DATA_TYPE>
class NodeFactory
{
public:

  NodeFactory() : m_rng()
  {

  }

  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>
  create(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples,
         unsigned int num_tests);

protected:

  virtual Node<LABEL_TYPE, DATA_TYPE>* createRandomNode() = 0;

  boost::random::random_device m_rng;

} ;

template<typename LABEL_TYPE, typename DATA_TYPE>
std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>>
NodeFactory<LABEL_TYPE, DATA_TYPE>::create(const std::vector<Sample<LABEL_TYPE, DATA_TYPE>>& samples,
                                           unsigned int num_tests)
{

  Histogram<LABEL_TYPE, DATA_TYPE> hist_samples(samples);

  std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_left;
  std::vector<Sample<LABEL_TYPE, DATA_TYPE>> samples_right;

  float max_info_gain = 0.;
  Node<LABEL_TYPE, DATA_TYPE>* max_node = nullptr;
  for (unsigned int i = 0; i < num_tests; ++i)
  {
    Node<LABEL_TYPE, DATA_TYPE>* node = createRandomNode();
    samples_left.clear();
    samples_right.clear();
    node->split(samples, samples_left, samples_right);

    Histogram<LABEL_TYPE, DATA_TYPE> hist_left(samples_left);
    Histogram<LABEL_TYPE, DATA_TYPE> hist_right(samples_right);

    float infoGain = hist_samples.informationGain({&hist_left, &hist_right});
    if (infoGain > max_info_gain)
    {
      max_info_gain = infoGain;
      max_node = node;
    }
  }

  std::unique_ptr<Node<LABEL_TYPE, DATA_TYPE>> p(max_node);
  return p;
}

#endif
