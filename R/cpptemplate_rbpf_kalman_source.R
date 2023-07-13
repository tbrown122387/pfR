cpptemp_rbpfkalman_source <-
'// replace every instance of <TODO> with your own code! 



classMODNAME::classMODNAME(<TODO>)
{
  // TODO: instantiate m_params, etc.
}


double classMODNAME::logMuEv (const ssvec &x21 )
{
  // <TODO>
}


auto classMODNAME::q1Samp(const ovec &y1) -> ssvec
{
  // <TODO>
}


auto classMODNAME::initKalmanMean(const ssvec &x21) -> nssvec
{
  // <TODO>
}

auto classMODNAME::initKalmanVar(const ssvec &x21) -> nsssMat
{
  // <TODO>
}


auto classMODNAME::q1Samp(const ssvec &x2tm1, const ovec &yt) -> ssvec 
{
  // <TODO>
}


double classMODNAME::logQ1Ev(const ssvec &x21, const ovec &y1)
{
  // <TODO>
}


double classMODNAME::logFEv(const ssvec &x2t, const ssvec &x2tm1)
{
  // <TODO>
}


double classMODNAME::logQEv(const ssvec &x2t, const ssvec &x2tm1, const ovec &yt )
{
  // <TODO>
}


void classMODNAME::updateKalman(kalmanType &aModel, const ovec &yt, const ssvec &x2t)
{
  // <TODO>
}
'
