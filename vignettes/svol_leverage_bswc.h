#ifndef SVOL_LEVERAGE_BSWC_H
#define SVOL_LEVERAGE_BSWC_H

#include "pf/bootstrap_filter_with_covariates.h" // the boostrap particle filter
#include "pf/rv_samp.h" 			                    // for sampling random numbers
#include "pf/rv_eval.h" 			                    // for evaluating densities and pmfs
#include "pf/resamplers.h" 		                  // for resampling classes

using namespace pf;
using namespace pf::filters;
using namespace pf::resamplers;

#define nparts_SVOL_LEVERAGE_BSWC 500    // number of particles
#define dimstate_SVOL_LEVERAGE_BSWC 1  // dimension of state vectors
#define dimobs_SVOL_LEVERAGE_BSWC 1    // dimension of observation vectors
#define dimcov_SVOL_LEVERAGE_BSWC 1    // dimension of covariate vectors
#define dimparam_SVOL_LEVERAGE_BSWC 4  // dimension of parameters

// helper type aliases
using resampT   = mn_resamp_fast1<nparts_SVOL_LEVERAGE_BSWC,dimstate_SVOL_LEVERAGE_BSWC,double>;
using svec      = Eigen::Matrix  <double, dimstate_SVOL_LEVERAGE_BSWC,1>;
using ovec      = Eigen::Matrix  <double, dimobs_SVOL_LEVERAGE_BSWC,1>;
using cvec      = Eigen::Matrix  <double, dimcov_SVOL_LEVERAGE_BSWC,1>;
using param_vec = Eigen::Matrix  <double, dimparam_SVOL_LEVERAGE_BSWC,1>;
using DynMat    = Eigen::Matrix  <double, Eigen::Dynamic, Eigen::Dynamic>;
using func      = std::function  <const DynMat(const svec&, const cvec&)>;
using BasePF    = BSFilterWC     <nparts_SVOL_LEVERAGE_BSWC, dimstate_SVOL_LEVERAGE_BSWC, dimobs_SVOL_LEVERAGE_BSWC, dimcov_SVOL_LEVERAGE_BSWC, resampT, double, true>;


/**
 * @brief a particle filter class template for a TODO model
 *
 */
class Svol_leverageBSWC : public BasePF
{
private:

  // example parameter
  param_vec m_params;

  // use this for sampling
  rvsamp::UnivNormSampler<double> m_example_stdNormSampler;

  // required by algorithm and required to define your model
  double logQ1Ev (const svec &x1,   const ovec &y1,  const cvec &z1 );
  double logMuEv (const svec &x1,   const cvec &z1                  );
  double logGEv  (const ovec &yt,   const svec &xt,  const cvec& zt );
  auto   q1Samp  (const ovec &y1,   const cvec& z1                  ) -> svec;
  auto   fSamp   (const svec &xtm1, const cvec &zt                  ) -> svec;

public:

  // constructor
  Svol_leverageBSWC(double phi, double mu, double sigma, double rho);

};

#endif  // SVOL_LEVERAGE_BSWC_H
