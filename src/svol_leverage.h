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
#define dimboth 1 // dimension of both state vector and observation vectors
#define dimparam 4

// helper type aliases
using resampT = mn_resamp_fast1<nparts_svol_lev,dimboth,double>;
using vec  = Eigen::Matrix<double,dimboth,1>;
using param_vec = Eigen::Matrix<double,dimparam,1>;
using DynMat = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>;
using func = std::function<const DynMat(const vec&, const vec&)>;
using BasePF = BSFilterWC<nparts_svol_lev, dimboth, dimboth, dimboth, resampT, double>;

/**
 * @brief a particle filter class template for a Hull-White stochastic volatility model
 *
 */
class svol_leverage : public BasePF
{
private:
  // parameters
  double m_phi;
  double m_mu;
  double m_sigma;
  double m_rho;
  param_vec m_untrans_params;

  // use this for sampling
  rvsamp::UnivNormSampler<double> m_stdNormSampler; 
  
  // required by bootstrap filter base class
  double logQ1Ev(const vec &x1, const vec &y1, const vec &z1);
  double logMuEv(const vec &x1, const vec &z1);
  double logGEv(const vec &yt, const vec &xt, const vec& zt);
  auto stateTransSamp(const vec &xtm1, const vec& zt) -> vec;
  auto q1Samp(const vec &y1, const vec& z1) -> vec;
  auto fSamp(const vec &xtm1, const vec &ytm1) -> vec;
  
  // what function of states you want to filter with
  std::vector<func> m_fs;
  
  
// methods that are exposable in R  
public:

  // constructor
  svol_leverage(double phi, double mu, double sigma, double rho);
  
  // // filter
  // void update(double current_obs, double lagged_obs);

};

#endif //SVOL_LEVERAGE_MOD_H
