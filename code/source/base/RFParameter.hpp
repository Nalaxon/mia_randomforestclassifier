#ifndef RFPARAMETER_HPP
#define RFPARAMETER_HPP

#include "RTParameter.hpp"
#include "Histogram.hpp"

#include <functional>
#include <vector>

/// class RFParameter - 
template<typename LABEL_TYPE, typename DATA_TYPE>
class RFParameter
{
    // Attributes
public:

    typedef std::function<Histogram<LABEL_TYPE, DATA_TYPE>(std::vector<Histogram<LABEL_TYPE, DATA_TYPE>>)> EnsembleFct;

    RFParameter()
    {
        m_bagging = true;
    }

    RFParameter(bool bagging, RTParameter treeParams,
                EnsembleFct ensemble_fct,
                unsigned int num_trees)
    {
        m_numTrees = num_trees;
        m_bagging = bagging;
        m_treeParams = treeParams;
        m_ensemble_fct = ensemble_fct;
    }

    EnsembleFct getEnsembleFct() const
    {
        return m_ensemble_fct;
    }

    const RTParameter& getTreeParams() const
    {
        return m_treeParams;
    }

    void setTreeParams(const RTParameter& treeParams)
    {
        m_treeParams = treeParams;
    }

    bool getBagging() const
    {
        return m_bagging;
    }

    void setBagging(bool bagging)
    {
        m_bagging = bagging;
    }

    unsigned int getNumTrees() const
    {
        return m_numTrees;
    }

private:
    RTParameter m_treeParams;
    EnsembleFct m_ensemble_fct;
    bool m_bagging;
    unsigned int m_numTrees;
};

#endif
