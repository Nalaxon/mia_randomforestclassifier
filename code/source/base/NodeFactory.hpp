#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"
#include "types.hpp"
#include "memory.hpp"
#include <fstream>


#include <boost/random/random_device.hpp>

// forward declaration for friend declaration
template<typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE >
class UniversalNodeFactory;


template<typename LABEL_TYPE, typename DATA_TYPE, typename ROI_TYPE>
class NodeFactory
{
  // this is necessary to get access to createRandomNode()...
	friend UniversalNodeFactory < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

public:

	using NodeType = Node < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

	using NodePtr = NodePtr_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

    using SampleType = Sample < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;
	using SampleVector = SampleVector_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

	using HistogramType = Histogram < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

	using HistogramPtr = HistogramPtr_ < LABEL_TYPE, DATA_TYPE, ROI_TYPE >;

  //----------------------------------------------------------------------------
  // Constructor
  //----------------------------------------------------------------------------
  NodeFactory() : m_rng()
  {
    static_assert(std::is_base_of<Label<LABEL_TYPE>, LABEL_TYPE>::value, "Type parameter LABEL_TYPE must derive from Label.");

	m_log_stream = new std::ofstream(std::string("../bin/NodeLog.txt").c_str(), std::ofstream::out);
  }

  NodeFactory(const NodeFactory& other)
    : m_rng()
  {
  }

  ~NodeFactory()
  {
	  if (m_log_stream != nullptr)
		  delete m_log_stream;
  }

  NodeFactory& operator=(const NodeFactory& other)
  {
    std::swap(*this, other);
    return *this;
  }

  //----------------------------------------------------------------------------
  NodePtr create(const SampleVector& samples, unsigned int num_tests)
  {
    auto hist_samples = std::make_unique<HistogramType>(samples);

	std::vector<SampleType> samples_left;
	std::vector<SampleType> samples_right;

    float max_info_gain = -0.1f;
    NodePtr max_node;
    for (unsigned int i = 0; i < num_tests; ++i)
    {
		NodePtr node(createRandomNode(m_log_stream));
      samples_left.clear();
      samples_right.clear();
      //node->setThreshold(samples);
      node->split(samples, samples_left, samples_right);

      HistogramType hist_left(samples_left);
      HistogramType hist_right(samples_right);

      std::vector<HistogramType*> hist;
      hist.push_back(&hist_left);
      hist.push_back(&hist_right);

      float infoGain = hist_samples->informationGain(hist);
	  *m_log_stream << ": IGain: " << infoGain << " / " << max_info_gain << ";" << i << ": " << samples_left.size() << "," << samples_right.size() << std::endl;
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
  virtual NodePtr createRandomNode(std::ostream* log_stream) = 0;
  //virtual NodePtr createRandomNode(std::ostream* log_stream, SampleType sample);
  virtual std::string get_ClassName() = 0;

  // a random number generator
  boost::random::random_device m_rng;

  std::ostream* m_log_stream;


private:
    const SampleType pickRandomSample(const SampleVector& samples)
    {
        boost::random::uniform_int_distribution<> index(0, samples.size() -1);
        return samples[index(m_rng)];
    }

};


#endif
