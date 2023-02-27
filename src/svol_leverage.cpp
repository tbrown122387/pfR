#include "svol_leverage.h" 


svol_leverage::svol_leverage(FLOATTYPE phi, FLOATTYPE mu, FLOATTYPE sigma, FLOATTYPE rho)
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
  FLOATTYPE mean = m_untrans_params(1) + m_untrans_params(0) * (xtm1(0) - m_untrans_params(1)) +
    zt(0) * m_untrans_params(3) * m_untrans_params(2) * std::exp(-.5 * xtm1(0));
  xt(0) = mean + m_stdNormSampler.sample() * m_untrans_params(2) * std::sqrt(1.0 - m_untrans_params(3) * m_untrans_params(3));
  return xt;
}


FLOATTYPE svol_leverage::logGEv(const vec &yt, const vec &xt, const vec& zt)
{
  return rveval::evalUnivNorm<FLOATTYPE>(
    yt(0),
    0.0,
    std::exp(.5 * xt(0)),
    true);
}

FLOATTYPE svol_leverage::logMuEv(const vec &x1, const vec& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<FLOATTYPE>(
    x1(0),
    0.0,
    m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0)),
    true);
}


FLOATTYPE svol_leverage::logQ1Ev(const vec &x1, const vec &y1, const vec& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<FLOATTYPE>(
    x1(0),
    0.0,
    m_untrans_params(2) / std::sqrt(1.0 - m_untrans_params(0) * m_untrans_params(0)),
    true);
}


void svol_leverage::update(FLOATTYPE current_obs, FLOATTYPE lagged_obs)
{
  vec newdat, lagdat;
  newdat << current_obs;
  lagdat << lagged_obs;
  this->filter(newdat, lagdat, m_fs);
}