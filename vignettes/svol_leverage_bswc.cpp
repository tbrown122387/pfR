Svol_leverageBSWC::Svol_leverageBSWC(double phi, double mu, double sigma, double rho)
{
  m_params << phi, mu, sigma, rho;
}


auto Svol_leverageBSWC::q1Samp(const ovec &y1, const cvec& z1) -> svec
{
  // phi, mu, sigma, rho
  svec x1samp;
  x1samp(0) = m_example_stdNormSampler.sample() * m_params(2) / std::sqrt(1.0 - m_params(0) * m_params(0));
  return x1samp;
}


auto Svol_leverageBSWC::fSamp(const svec &xtm1, const cvec& zt) -> svec
{
  // phi, mu, sigma, rho
  svec xt;
  double mean = m_params(1) + m_params(0) * (xtm1(0) - m_params(1)) +
    zt(0) * m_params(3) * m_params(2) * std::exp(-.5 * xtm1(0));
  xt(0) = mean + m_example_stdNormSampler.sample() * m_params(2) * std::sqrt(1.0 - m_params(3) * m_params(3));
  return xt;
}


double Svol_leverageBSWC::logGEv(const ovec &yt, const svec &xt, const cvec& zt)
{
  return rveval::evalUnivNorm<double>(
    yt(0),
    0.0,
    std::exp(.5 * xt(0)),
    true);
}


double Svol_leverageBSWC::logMuEv(const svec &x1, const cvec& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<double>(
    x1(0),
    0.0,
    m_params(2) / std::sqrt(1.0 - m_params(0) * m_params(0)),
    true);
}


double Svol_leverageBSWC::logQ1Ev(const svec &x1, const ovec &y1, const cvec& z1)
{
  // parameter order phi, mu, sigma, rho
  return rveval::evalUnivNorm<double>(
    x1(0),
    0.0,
    m_params(2) / std::sqrt(1.0 - m_params(0) * m_params(0)),
    true);
}

