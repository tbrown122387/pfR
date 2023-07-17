double mean_factor_msvol_apf_approx_LL(const Rcpp::NumericMatrix& obsTS, const Rcpp::NumericVector& params){

  // // instantiate model with arg params
  if(params.size() != dimparam_MEAN_FACTOR_MSVOL_APF) Rcpp::stop("Wrong size");
  param_vec eig_params;
  std::memcpy(&eig_params[0,0], &params[0], dimparam_MEAN_FACTOR_MSVOL_APF*sizeof(double));
  Mean_factor_msvol_apf mod(eig_params);

  // calculate likelihood by iterating through time series
  double ans(0.0);
  ovec yt;
  for (int i = 0; i < obsTS.rows(); i++){
    yt << obsTS(i,0), obsTS(i,1);
    mod.filter(yt);
    ans += mod.getLogCondLike();
  }

  return ans;
}


Rcpp::NumericMatrix mean_factor_msvol_apf_approx_filt(
    const Rcpp::NumericMatrix& obsTS,const Rcpp::NumericVector& params){
  
  // // instantiate model with arg params
  if(params.size() != dimparam_MEAN_FACTOR_MSVOL_APF) Rcpp::stop("Wrong size");
  param_vec eig_params;
  std::memcpy(&eig_params[0,0], &params[0], dimparam_MEAN_FACTOR_MSVOL_APF*sizeof(double));
  Mean_factor_msvol_apf mod(eig_params);
  
  // instantiate container for filter means
  Eigen::MatrixXd ans2(obsTS.rows(), dimstate_MEAN_FACTOR_MSVOL_APF);
  
  // create a function to evaluate on particle samples
  std::vector<func> pf_fs;
  pf_fs.emplace_back([](const svec& xt)-> DynMat{ return xt; });
  
  // iterate through data and save filter means
  ovec yt;
  for (int i = 0; i < obsTS.rows(); i++){
    yt << obsTS(i,0), obsTS(i,1);
    mod.filter(yt, pf_fs);
    ans2.row(i) = mod.getExpectations()[0].col(0).transpose();
  }
  
  // convert stuff to Module-friendly type
  SEXP s = Rcpp::wrap(ans2);
  Rcpp::NumericMatrix ans(s);
  return ans;
}

