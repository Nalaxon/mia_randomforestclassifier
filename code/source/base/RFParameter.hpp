#ifndef RFPARAMETER_HPP
#define RFPARAMETER_HPP

#include "RTParameter.hpp"
#include "Histogram.hpp"

#include <functional>
#include <vector>

template<typename LABEL_TYPE, typename DATA_TYPE>
class RFParameter
{
public:

  using HistogramType = Histogram<LABEL_TYPE, DATA_TYPE>;
  
  using HistogramVector = HistogramVector_<LABEL_TYPE, DATA_TYPE>;
  
  using EnsembleFct = std::function<HistogramType(const HistogramVector&)>;

  //----------------------------------------------------------------------------
  RFParameter(bool bagging, RTParameter treeParams,
              EnsembleFct ensemble_fct,
              unsigned int num_trees) : m_numTrees(num_trees),
                                        m_bagging(bagging),
                                        m_treeParams(treeParams),
                                        m_ensemble_fct(ensemble_fct)
  {
  }

  //----------------------------------------------------------------------------
  EnsembleFct getEnsembleFct() const
  {
    return m_ensemble_fct;
  }

  //----------------------------------------------------------------------------
  const RTParameter& getTreeParams() const
  {
    return m_treeParams;
  }

  //----------------------------------------------------------------------------
  void setTreeParams(const RTParameter& treeParams)
  {
    m_treeParams = treeParams;
  }

  //----------------------------------------------------------------------------
  bool getBagging() const
  {
    return m_bagging;
  }

  //----------------------------------------------------------------------------
  void setBagging(bool bagging)
  {
    m_bagging = bagging;
  }

  //----------------------------------------------------------------------------
  unsigned int getNumTrees() const
  {
    return m_numTrees;
  }

private:
  
  // number of trees to use
  unsigned int m_numTrees;
  
  // is bagging enabled?
  bool m_bagging;
  
  // tree parameters
  RTParameter m_treeParams;
  
  // the ensemble function
  EnsembleFct m_ensemble_fct;  
} ;

#endif
