// replace every instance of <TODO> with your own code! 

#include "mean_factor_msvol_aux.h" 

Mean_factor_msvolAUX::Mean_factor_msvolAUX(std::vector<double> params)
{
  m_params = Eigen::Map<param_vec>(params.data(), params.size());
}


auto Mean_factor_msvolAUX::getB() -> loadMat
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
    B.block(c+1,c,num_rows,1) = m_params.segment(start_pos,num_rows);
    start_pos += num_rows;
  }

  return B;
}


auto Mean_factor_msvolAUX::getMu() -> svec
{
  return m_params.segment(dimB, dimstate_MEAN_FACTOR_MSVOL_AUX);
}


auto Mean_factor_msvolAUX::getPhi() -> stateMat
{
  unsigned start_pos = dimB + dimstate_MEAN_FACTOR_MSVOL_AUX;
  return m_params.segment(start_pos, dimstate_MEAN_FACTOR_MSVOL_AUX).asDiagonal();
}



auto Mean_factor_msvolAUX::getSigma() -> stateMat
{
  unsigned start_pos = dimB + 2*dimstate_MEAN_FACTOR_MSVOL_AUX;
  return m_params.segment(start_pos, dimstate_MEAN_FACTOR_MSVOL_AUX).asDiagonal();
}


double Mean_factor_msvolAUX::logMuEv (const svec &x1 )
{
  // all sigma^2/(1-phi^2)
  stateMat cov = (getSigma().diagonal().array()/ (1.0 - getPhi().diagonal().array().square())).matrix().asDiagonal();
  return rveval::evalMultivNorm<dimstate_MEAN_FACTOR_MSVOL_AUX, double>(x1, svec::Zero(), cov, true);
}


auto Mean_factor_msvolAUX::propMu(const svec &xtm1) -> svec
{
  return getMu() + getPhi() * ( xtm1 - getMu() );
}


auto Mean_factor_msvolAUX::q1Samp(const ovec &y1) -> svec
{
  stateMat cov = (getSigma().diagonal().array()/ (1.0 - getPhi().diagonal().array().square())).matrix().asDiagonal();
  m_mvnSampler.setCovar(cov);
  return m_mvnSampler.sample();
}


auto Mean_factor_msvolAUX::fSamp(const svec &xtm1) -> svec
{
  m_mvnSampler.setCovar(getSigma());
  m_mvnSampler.setMean(getMu() + getPhi() * ( xtm1 - getMu() ));
  return m_mvnSampler.sample();
}


double Mean_factor_msvolAUX::logQ1Ev (const svec &x1, const ovec &y1)
{
  stateMat cov = (getSigma().diagonal().array()/ (1.0 - getPhi().diagonal().array().square())).matrix().asDiagonal();
  return rveval::evalMultivNorm<dimstate_MEAN_FACTOR_MSVOL_AUX,double>(x1, svec::Zero(), cov, true);
}


double Mean_factor_msvolAUX::logGEv (const ovec &yt, const svec &xt)
{
  obsMat V = (xt.segment(0,dimobs_MEAN_FACTOR_MSVOL_AUX).array().exp()).matrix().asDiagonal();
  factorMat D = (xt.segment(dimobs_MEAN_FACTOR_MSVOL_AUX,dimfactor_MEAN_FACTOR_MSVOL_AUX).array().exp()).matrix().asDiagonal();
  obsMat cov = getB() * D * getB().transpose() + V;
  return rveval::evalMultivNorm<dimobs_MEAN_FACTOR_MSVOL_AUX,double>(yt, ovec::Zero(), cov, true);
}



