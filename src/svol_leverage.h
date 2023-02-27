#ifndef SVOL_LEVERAGE_MOD_H
#define SVOL_LEVERAGE_MOD_H

#include "pf/include/pf/bootstrap_filter_with_covariates.h" // the boostrap particle filter
#include "pf/include/pf/rv_samp.h" 			      // for sampling random numbers
#include "pf/include/pf/rv_eval.h" 			      // for evaluating densities and pmfs
#include "pf/include/pf/resamplers.h" 			      // for resampling classes

using namespace pf;
using namespace pf::filters;
using namespace pf::resamplers;

#define nparts_svol_lev 500
#define dimstate 1
#define dimobs 1
#define dimparam 4
using FLOATTYPE = double;
using resampT = mn_resamp_fast1<nparts_svol_lev,dimstate,FLOATTYPE>;

// helper type aliases
using state_vector  = Eigen::Matrix<FLOATTYPE,dimstate,1>;
using obs_vector  = Eigen::Matrix<FLOATTYPE,dimobs,1>;
using param_vector  = Eigen::Matrix<FLOATTYPE,dimparam,1>;

/**
 * @brief a particle filter class template for a Hull-White stochastic volatility model
 *
 */
class svol_leverage : public BSFilterWC<nparts_svol_lev, dimstate, dimobs, dimobs, resampT, FLOATTYPE>
{
private:
  // parameters
  FLOATTYPE m_phi;
  FLOATTYPE m_mu;
  FLOATTYPE m_sigma;
  FLOATTYPE m_rho;
  param_vector m_untrans_params;

  // use this for sampling
  rvsamp::UnivNormSampler<FLOATTYPE> m_stdNormSampler; 
  
  // required by bootstrap filter base class
  FLOATTYPE logQ1Ev(const state_vector &x1, const obs_vector &y1, const obs_vector &z1);
  FLOATTYPE logMuEv(const state_vector &x1, const obs_vector &z1);
  FLOATTYPE logGEv(const obs_vector &yt, const state_vector &xt, const obs_vector& zt);
  auto stateTransSamp(const state_vector &xtm1, const obs_vector& zt) -> state_vector;
  auto q1Samp(const obs_vector &y1, const obs_vector& z1) -> state_vector;
  auto fSamp(const state_vector &xtm1, const obs_vector &ytm1) -> state_vector;

// methods that are exposable in R  
public:

  // constructor
  svol_leverage(const FLOATTYPE &phi, const FLOATTYPE &mu, const FLOATTYPE &sigma, const FLOATTYPE& rho);
  
  // // wrapper to return log p(y_t | y_{1:t-1})
  // FLOATTYPE lastLogCondLike();
  
};

#endif //SVOL_LEVERAGE_MOD_H
