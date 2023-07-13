cpptemp_rbpfkalman_header <- 
'// replace every instance of <TODO> with your own code! 

#ifndef HEADERGUARDMODNAME_H
#define HEADERGUARDMODNAME_H

#include "pf/rbpf.h"             // the auxiliary particle filter
#include "pf/rv_samp.h"          // for sampling random numbers
#include "pf/rv_eval.h"          // for evaluating densities and pmfs
#include "pf/resamplers.h"       // for resampling classes
#include "pf/cf_filters.h"       // for closed-form filter types

using namespace pf;
using namespace pf::filters;
using namespace pf::resamplers;

#define nparts_MODNAME <TODO>      // number of particles
#define dimnsstate_MODNAME <TODO>  // dimension of state sub-vector *not* sampled
#define dimsstate_MODNAME <TODO>   // dimension of state sub-vector that *is* sampled
#define dimobs_MODNAME <TODO>      // dimension of observation vectors
#define dimparam_MODNAME <TODO>    // dimension of parameters

// helper type aliases
using kalmanType= kalman<dimnsstate_MODNAME,dimobs_MODNAME,0,double>;
using resampT   = mn_resampler_rbpf<nparts_MODNAME,dimsstate_MODNAME,kalmanType,double>;
using nssvec    = Eigen::Matrix    <double, dimnsstate_MODNAME,1>;
using nsssMat   = Eigen::Matrix    <double, dimnsstate_MODNAME,dimnsstate_MODNAME>;
using ssvec     = Eigen::Matrix    <double, dimsstate_MODNAME,1>;
using ovec      = Eigen::Matrix    <double, dimobs_MODNAME,1>;
using param_vec = Eigen::Matrix    <double, dimparam_MODNAME,1>;
using DynMat    = Eigen::Matrix    <double, Eigen::Dynamic, Eigen::Dynamic>;
using func      = std::function    <const DynMat(const nssvec&,const ssvec&)>;
using BasePF    = rbpf_hmm         <nparts_MODNAME,dimnsstate_MODNAME,dimsstate_MODNAME,dimobs_MODNAME,resampT,double>;


/**
 * @brief a particle filter class template for a TODO model
 *
 */
class classMODNAME : public BasePF
{
private:

  // example parameter
  param_vec m_params;

  // use this for sampling
  rvsamp::UnivNormSampler<double> m_example_stdNormSampler; 
  
  // required by algorithm and required to define your model
  double logMuEv (const ssvec &x21                )        ;
  auto   q1Samp  (const ovec &y1                ) -> ssvec;
  auto   initKalmanMean(const ssvec &x21) -> nssvec ;
  auto   initKalmanVar(const ssvec &x21) ->  nsssMat;
  auto   qSamp(const ssvec &x2tm1, const ovec &yt) -> ssvec ;
  double logQ1Ev(const ssvec &x21, const ovec &y1);
  double logFEv(const ssvec &x2t, const ssvec &x2tm1);
  double logQEv(const ssvec &x2t, const ssvec &x2tm1, const ovec &yt );
  void updateKalman(kalmanType &aModel, const ovec &yt, const ssvec &x2t);
  

public:

  // constructor
  classMODNAME(<TODO>);
  
};

#endif  // HEADERGUARDMODNAME_H'
