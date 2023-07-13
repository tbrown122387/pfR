cpptemp_sisr_header <- 
'// replace every instance of <TODO> with your own code! 

#ifndef HEADERGUARDMODNAME_H
#define HEADERGUARDMODNAME_H

#include "pf/sisr_filter.h"     // the auxiliary particle filter
#include "pf/rv_samp.h"         // for sampling random numbers
#include "pf/rv_eval.h"         // for evaluating densities and pmfs
#include "pf/resamplers.h"      // for resampling classes

using namespace pf;
using namespace pf::filters;
using namespace pf::resamplers;

#define nparts_MODNAME <TODO>    // number of particles
#define dimstate_MODNAME <TODO>  // dimension of state vectors
#define dimobs_MODNAME <TODO>    // dimension of observation vectors
#define dimparam_MODNAME <TODO>  // dimension of parameters

// helper type aliases
using resampT   = mn_resamp_fast1<nparts_MODNAME,dimstate_MODNAME,double>;
using svec      = Eigen::Matrix  <double, dimstate_MODNAME,1>;
using ovec      = Eigen::Matrix  <double, dimobs_MODNAME,1>;
using param_vec = Eigen::Matrix  <double, dimparam_MODNAME,1>;
using DynMat    = Eigen::Matrix  <double, Eigen::Dynamic, Eigen::Dynamic>;
using func      = std::function  <const DynMat(const svec&)>;
using BasePF    = SISRFilter		 <nparts_MODNAME,dimstate_MODNAME,dimobs_MODNAME,resampT,double>;


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
  double logMuEv (const svec &x1                                  )        ;
  auto   q1Samp  (const ovec &y1                                  ) -> svec;
  double logQ1Ev (const svec &x1, const ovec &y1                  )        ;
  double logGEv  (const ovec &yt, const svec &xt                  )        ;
  double logFEv  (const svec &xt, const svec &xtm1                )        ;
  auto   qSamp   (const svec &xtm1, const ovec &yt                ) -> svec;
  double logQEv  (const svec &xt, const svec &xtm1, const ovec &yt)        ;
  
public:

  // constructor
  classMODNAME(<TODO>);
  
};

#endif  // HEADERGUARDMODNAME_H'
