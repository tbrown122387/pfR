#include "svol_leverage.h" 


svol_leverage::svol_leverage(const FLOATTYPE &phi, const FLOATTYPE &mu, const FLOATTYPE &sigma, const FLOATTYPE &rho)
  : m_phi(phi), m_mu(mu), m_sigma(sigma), m_rho(rho)
{
  m_untrans_params << m_phi, m_mu, m_sigma, m_rho;
}


auto svol_leverage::q1Samp(const obs_vector &y1, const obs_vector& z1) -> state_vector
{
  // phi, mu, sigma, rho
  state_vector x1samp;
  x1samp(0) = m_stdNormSampler.sample() * m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0));
  return x1samp;
}


auto svol_leverage::fSamp(const state_vector &xtm1, const obs_vector& zt) -> state_vector
{
  // phi, mu, sigma, rho
  state_vector xt;
  FLOATTYPE mean = m_untrans_params(1) + m_untrans_params(0) * (xtm1(0) - m_untrans_params(1)) +
    zt(0) * m_untrans_params(3) * m_untrans_params(2) * std::exp(-.5 * xtm1(0));
  xt(0) = mean + m_stdNormSampler.sample() * m_untrans_params(2) * std::sqrt(1.0 - m_untrans_params(3) * m_untrans_params(3));
  return xt;
}


FLOATTYPE svol_leverage::logGEv(const obs_vector &yt, const state_vector &xt, const obs_vector& zt)
{
  return rveval::evalUnivNorm<FLOATTYPE>(
    yt(0),
    0.0,
    std::exp(.5 * xt(0)),
    true);
}

FLOATTYPE svol_leverage::logMuEv(const state_vector &x1, const obs_vector& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<FLOATTYPE>(
    x1(0),
    0.0,
    m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0)),
    true);
}


FLOATTYPE svol_leverage::logQ1Ev(const state_vector &x1, const obs_vector &y1, const obs_vector& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<FLOATTYPE>(
    x1(0),
    0.0,
    m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0)),
    true);
}


// FLOATTYPE svol_leverage::lastLogCondLike()
// {
//   return getLogCondLike();
// }

