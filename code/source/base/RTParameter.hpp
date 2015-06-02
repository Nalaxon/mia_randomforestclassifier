#ifndef RTPARAMETER_HPP
#define RTPARAMETER_HPP

/// class RTParameter - 
class RTParameter {
  // Attributes
private:
  size_t m_maxDepth;
  size_t m_minSamples;

public:
	RTParameter(size_t maxDepth, size_t minSamples)
	{
		m_maxDepth = maxDepth;
		m_minSamples = minSamples;
	}

	size_t getMaxDepth()
	{
		return m_maxDepth;
	}

	void setMaxDepth(size_t maxDepth)
	{
		m_maxDepth = maxDepth;
	}

	size_t getMinSamples()
	{
		return m_minSamples;
	}

	void setMinSamples(size_t minSamples)
	{
		m_minSamples = minSamples;
	}
};

#endif
