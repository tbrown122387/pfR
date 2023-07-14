// replace every instance of <TODO> with your own code! 

#ifndef MEAN_FACTOR_MSVOL_APF_H
#define MEAN_FACTOR_MSVOL_APF_H

#include "pf/auxiliary_pf.h"     // the auxiliary particle filter
#include "pf/rv_samp.h"          // for sampling random numbers
#include "pf/rv_eval.h"          // for evaluating densities and pmfs
#include "pf/resamplers.h"       // for resampling classes

using namespace pf;
using namespace pf::filters;
using namespace pf::resamplers;

#define nparts_MEAN_FACTOR_MSVOL_APF 100    // number of particles
#define dimobs_MEAN_FACTOR_MSVOL_APF 2      // dimension of observation vectors (aka p)
#define dimfactor_MEAN_FACTOR_MSVOL_APF 1   // dimension of factor (aka q)
#define dimstate_MEAN_FACTOR_MSVOL_APF 3    // sum of number of obs and number of factors
#define dimparam_MEAN_FACTOR_MSVOL_APF 10   // pq - (q+1)*q/2 + 3*(p+q)
#define dimB 1


// helper type aliases
using resampT   = mn_resamp_fast1<nparts_MEAN_FACTOR_MSVOL_APF,dimstate_MEAN_FACTOR_MSVOL_APF,double>;
using svec      = Eigen::Matrix  <double, dimstate_MEAN_FACTOR_MSVOL_APF,1>;
using ovec      = Eigen::Matrix  <double, dimobs_MEAN_FACTOR_MSVOL_APF,1>;
using param_vec = Eigen::Matrix  <double, dimparam_MEAN_FACTOR_MSVOL_APF,1>;
using DynMat    = Eigen::Matrix  <double, Eigen::Dynamic, Eigen::Dynamic>;
using func      = std::function  <const DynMat(const svec&)>;
using BasePF    = APF<nparts_MEAN_FACTOR_MSVOL_APF,dimstate_MEAN_FACTOR_MSVOL_APF,dimobs_MEAN_FACTOR_MSVOL_APF,resampT,double>;

// extra type aliases to help shape parameters
using loadMat   = Eigen::Matrix <double, dimobs_MEAN_FACTOR_MSVOL_APF, dimfactor_MEAN_FACTOR_MSVOL_APF>;
using stateMat  = Eigen::Matrix <double, dimstate_MEAN_FACTOR_MSVOL_APF, dimstate_MEAN_FACTOR_MSVOL_APF>;
using obsMat    = Eigen::Matrix<double, dimobs_MEAN_FACTOR_MSVOL_APF, dimobs_MEAN_FACTOR_MSVOL_APF>;
using factorMat = Eigen::Matrix<double, dimfactor_MEAN_FACTOR_MSVOL_APF,dimfactor_MEAN_FACTOR_MSVOL_APF>;


/**
 * @brief A mean factor msvol model using an auxiliary particle filter
 *
 */
class Mean_factor_msvol_apf : public BasePF
{
private:

  // param order: B, mu, Phi, Sigma
  loadMat  m_B;
  svec     m_Mu;
  stateMat m_Phi;
  stateMat m_Sigma;
  stateMat m_initVar;
  
  // use this for sampling
  rvsamp::MVNSampler<dimstate_MEAN_FACTOR_MSVOL_APF,double> m_timeOneSampler;
  rvsamp::MVNSampler<dimstate_MEAN_FACTOR_MSVOL_APF,double> m_transitionSampler;
  
  // helper functions to shape parameters
  auto getB    (const param_vec& theta) -> loadMat;
  auto getMu   (const param_vec& theta) -> svec;
  auto getPhi  (const param_vec& theta) -> stateMat;
  auto getSigma(const param_vec& theta) -> stateMat;
  
  
  // required by algorithm and required to define your model
  double logMuEv (const svec &x1                )        ;
  auto   propMu  (const svec &xtm1              ) -> svec;
  auto   q1Samp  (const ovec &y1                ) -> svec;
  auto   fSamp   (const svec &xtm1              ) -> svec;
  double logQ1Ev (const svec &x1, const ovec &y1)        ;
  double logGEv  (const ovec &yt, const svec &xt)        ;
  
public:

  // constructor
  Mean_factor_msvol_apf(const param_vec& params);
  
};

#endif  // MEAN_FACTOR_MSVOL_APF_H
