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
#define FLOATTYPE double

// helper type aliases
using resampT = mn_resamp_fast1<nparts_svol_lev,dimstate,FLOATTYPE>;
using vec  = Eigen::Matrix<FLOATTYPE,dimstate,1>;
using vec  = Eigen::Matrix<FLOATTYPE,dimobs,1>;
using param_vec = Eigen::Matrix<FLOATTYPE,dimparam,1>;
using DynMat = Eigen::Matrix<FLOATTYPE,Eigen::Dynamic,Eigen::Dynamic>;
using func = std::function<const DynMat(const vec&, const vec&)>;


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
  param_vec m_untrans_params;

  // use this for sampling
  rvsamp::UnivNormSampler<FLOATTYPE> m_stdNormSampler; 
  
  // required by bootstrap filter base class
  FLOATTYPE logQ1Ev(const vec &x1, const vec &y1, const vec &z1);
  FLOATTYPE logMuEv(const vec &x1, const vec &z1);
  FLOATTYPE logGEv(const vec &yt, const vec &xt, const vec& zt);
  auto stateTransSamp(const vec &xtm1, const vec& zt) -> vec;
  auto q1Samp(const vec &y1, const vec& z1) -> vec;
  auto fSamp(const vec &xtm1, const vec &ytm1) -> vec;
  
  // what function of states you want to filter with
  std::vector<func> m_fs;
  
  
// methods that are exposable in R  
public:

  // constructor
  svol_leverage(FLOATTYPE phi, FLOATTYPE mu, FLOATTYPE sigma, FLOATTYPE rho);
  
  // filter
  void update(FLOATTYPE current_obs, FLOATTYPE lagged_obs);

};

#endif //SVOL_LEVERAGE_MOD_H
