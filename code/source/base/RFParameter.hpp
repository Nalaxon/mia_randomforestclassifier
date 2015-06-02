#ifndef RFPARAMETER_HPP
#define RFPARAMETER_HPP

#include "RTParameter.hpp"
#include "Histogram.hpp"

#include <functional>
#include <vector>

/// class RFParameter - 
template <typename LABEL_TYPE, typename DATA_TYPE>
class RFParameter {
  // Attributes
public:
	RFParameter()
	{
		m_bagging = true;
	}

	RFParameter(bool bagging, RTParameter treeParams, std::function < double(std::vector < Histrogram<LABEL_TYPE, DATA_TYPE)> ensemble_fct)
	{
		m_bagging = bagging;
		m_treeParams = treeParams;
		m_ensemble_fct = ensemble_fct;
	}

	RTParameter getTreeParams()
	{
		return m_treeParams;
	}

	void setTreeParams(RTParameter& treeParams)
	{
		m_treeParams = treeParams;
	}

	bool getBagging()
	{
		return m_bagging;
	}

	void setBagging(bool bagging)
	{
		m_bagging = bagging;
	}

private:
  RTParameter m_treeParams;
  std::function<double(std::vector<Histrogram<LABEL_TYPE, DATA_TYPE)> m_ensemble_fct;
  bool m_bagging;
};

#endif
