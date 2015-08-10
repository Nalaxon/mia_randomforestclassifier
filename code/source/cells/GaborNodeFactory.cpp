#include "GaborNodeFactory.hpp"
#include "GaborNode.hpp"
#include "NodeFactory.hpp"
#include "memory.hpp"

GaborNodeFactory::NodePtr GaborNodeFactory::createRandomNode(std::ostream* log_stream)
{
    double threshold = m_threshold_real_dist(m_rng);

	double sig = m_sig_th_lm_ps_dist(m_rng);
	double th = m_sig_th_lm_ps_dist(m_rng);
	double lm = m_sig_th_lm_ps_dist(m_rng);
	double gm = m_gm_dist(m_rng);
	double ps = m_sig_th_lm_ps_dist(m_rng);
	return std::make_unique<GaborNode>(threshold, sig, th, lm, gm, ps, log_stream);
}


