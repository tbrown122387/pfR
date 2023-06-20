#include "mean_factor_msvol_aux.h" 

Mean_factor_msvolAUX::Mean_factor_msvolAUX(const param_vec& params)
  : m_B(getB(params))
  , m_Mu(getMu(params))
  , m_Phi(getPhi(params))
  , m_Sigma(getSigma(params))
{
  // m_params = Eigen::Map<param_vec>(params.data(), params.size());
  m_initVar = (m_Sigma.diagonal().array()/ (1.0 - m_Phi.diagonal().array().square())).matrix().asDiagonal();
  
  // set up time 1 sampler
  m_timeOneSampler.setCovar(m_initVar);
  
  // set up transition sampler
  m_timeOneSampler.setCovar(m_Sigma);
}


auto Mean_factor_msvolAUX::getB(const param_vec& theta) -> loadMat
{
  loadMat B = loadMat::Zero();

  // set diagonals to 1 for identifiability
  for(unsigned i = 0; i < dimfactor_MEAN_FACTOR_MSVOL_AUX; ++i){
    B(i,i) = 1.0;
  }
  
  // take first pq - (q+1)*q/2 parameters and string them out by column
  unsigned start_pos = 0;
  for(unsigned c = 0; c < dimfactor_MEAN_FACTOR_MSVOL_AUX; ++c){
    unsigned num_rows = dimobs_MEAN_FACTOR_MSVOL_AUX - c - 1;
    B.block(c+1,c,num_rows,1) = theta.segment(start_pos,num_rows);
    start_pos += num_rows;
  }

  return B;
}


auto Mean_factor_msvolAUX::getMu(const param_vec& theta) -> svec
{
  return theta.segment(dimB, dimstate_MEAN_FACTOR_MSVOL_AUX);
}


auto Mean_factor_msvolAUX::getPhi(const param_vec& theta) -> stateMat
{
  unsigned start_pos = dimB + dimstate_MEAN_FACTOR_MSVOL_AUX;
  return theta.segment(start_pos, dimstate_MEAN_FACTOR_MSVOL_AUX).asDiagonal();
}



auto Mean_factor_msvolAUX::getSigma(const param_vec& theta) -> stateMat
{
  unsigned start_pos = dimB + 2*dimstate_MEAN_FACTOR_MSVOL_AUX;
  return theta.segment(start_pos, dimstate_MEAN_FACTOR_MSVOL_AUX).asDiagonal();
}


double Mean_factor_msvolAUX::logMuEv (const svec &x1 )
{
  // all sigma^2/(1-phi^2)
  return rveval::evalMultivNorm<dimstate_MEAN_FACTOR_MSVOL_AUX, double>(x1, 
                                                                        svec::Zero(), 
                                                                        m_initVar, 
                                                                        true);
}


auto Mean_factor_msvolAUX::propMu(const svec &xtm1) -> svec
{
  return m_Mu + m_Phi * ( xtm1 - m_Mu );
}


auto Mean_factor_msvolAUX::q1Samp(const ovec &y1) -> svec
{
  return m_timeOneSampler.sample();
}


auto Mean_factor_msvolAUX::fSamp(const svec &xtm1) -> svec
{
  return m_transitionSampler.sample();
}


double Mean_factor_msvolAUX::logQ1Ev (const svec &x1, const ovec &y1)
{
  return rveval::evalMultivNorm<dimstate_MEAN_FACTOR_MSVOL_AUX,double>(x1, 
                                                                       svec::Zero(), 
                                                                       m_initVar, 
                                                                       true);
}


double Mean_factor_msvolAUX::logGEv (const ovec &yt, const svec &xt)
{
  obsMat V = (xt.segment(0,dimobs_MEAN_FACTOR_MSVOL_AUX).array().exp()).matrix().asDiagonal();
  factorMat D = (xt.segment(dimobs_MEAN_FACTOR_MSVOL_AUX,dimfactor_MEAN_FACTOR_MSVOL_AUX).array().exp()).matrix().asDiagonal();
  obsMat cov = m_B * D * m_B.transpose() + V;
  return rveval::evalMultivNorm<dimobs_MEAN_FACTOR_MSVOL_AUX,double>(yt, ovec::Zero(), cov, true);
}



