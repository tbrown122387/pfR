cpptemp_auxpf_export <- 
r"(// replace every instance of <TODO> with your own code! 

double modname_approx_LL(const Rcpp::NumericVector& obsTS, const Rcpp::NumericVector& params){

  // instantiate model with arg params
  classMODNAME mod(<TODO>);

  // calculate likelihood by iterating through time series
  double ans(0.0);
  ovec yt;
  for (int i = 0; i < obsTS.length(); i++){
    yt << obsTS[i];
    mod.filter(yt);
    ans += mod.getLogCondLike();
  }
  return ans;
}


Rcpp::NumericVector modname_approx_filt(
    const Rcpp::NumericVector& obsTS,
    const Rcpp::NumericVector& params){
  
  // instantiate model with arg params
  classMODNAME mod(<TODO>);

  // instantiate container for filter means
  Rcpp::NumericVector ans(obsTS.size());

  // create a function to evaluate on particle samples
  std::vector<func> pf_fs;
  pf_fs.emplace_back([](const svec& xt)-> DynMat{ return xt; });

  // iterate through data and save filter means
  ovec yt;
  for (int i = 0; i < obsTS.length(); i++){
    yt << obsTS[i];
    mod.filter(yt, pf_fs);
    ans[i] = mod.getExpectations()[0](0,0);
  }
  return ans;
}
)"

