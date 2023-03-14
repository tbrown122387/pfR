// this file will contain 
// 1. definitions for declared class

#include "svol_leverage.h" 


svol_leverage::svol_leverage(double phi, double mu, double sigma, double rho)
  : m_phi(phi), m_mu(mu), m_sigma(sigma), m_rho(rho)
{
  m_untrans_params << m_phi, m_mu, m_sigma, m_rho;
  m_fs.emplace_back(
    [](const vec& xt, const vec& zt)-> DynMat { 
        return xt; 
      }
  );
  
}


auto svol_leverage::q1Samp(const vec &y1, const vec& z1) -> vec
{
  // phi, mu, sigma, rho
  vec x1samp;
  x1samp(0) = m_stdNormSampler.sample() * m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0));
  return x1samp;
}


auto svol_leverage::fSamp(const vec &xtm1, const vec& zt) -> vec
{
  // phi, mu, sigma, rho
  vec xt;
  double mean = m_untrans_params(1) + m_untrans_params(0) * (xtm1(0) - m_untrans_params(1)) +
    zt(0) * m_untrans_params(3) * m_untrans_params(2) * std::exp(-.5 * xtm1(0));
  xt(0) = mean + m_stdNormSampler.sample() * m_untrans_params(2) * std::sqrt(1.0 - m_untrans_params(3) * m_untrans_params(3));
  return xt;
}


double svol_leverage::logGEv(const vec &yt, const vec &xt, const vec& zt)
{
  return rveval::evalUnivNorm<double>(
    yt(0),
    0.0,
    std::exp(.5 * xt(0)),
    true);
}

double svol_leverage::logMuEv(const vec &x1, const vec& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<double>(
    x1(0),
    0.0,
    m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0)),
    true);
}


double svol_leverage::logQ1Ev(const vec &x1, const vec &y1, const vec& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<double>(
    x1(0),
    0.0,
    m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0)),
    true);
}


// void svol_leverage::update(double current_obs, double lagged_obs)
// {
//   vec newdat, lagdat;
//   newdat << current_obs;
//   lagdat << lagged_obs;
//   this->filter(newdat, lagdat, m_fs);
// }