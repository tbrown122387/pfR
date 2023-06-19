// replace every instance of <TODO> with your own code! 

#include "mean_factor_msvol_aux.h" 

//' @useDynLib pfr, .registration = TRUE
//' @import RcppEigen
//' @importFrom Rcpp evalCpp
//' @export
// [[Rcpp::export]]
double mean_factor_msvol_aux_approx_LL(const Rcpp::NumericMatrix& obsTS, const Rcpp::NumericVector& params){

  // instantiate model with arg params
  std::vector<double> p = Rcpp::as<std::vector<double> >(params);
  Mean_factor_msvolAUX mod(p);

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


//' @useDynLib pfr, .registration = TRUE
//' @import RcppEigen
//' @importFrom Rcpp evalCpp
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector mean_factor_msvol_aux_approx_filt(
    const Rcpp::NumericMatrix& obsTS,
    const Rcpp::NumericVector& params){
  
  // instantiate model with arg params
  std::vector<double> p = Rcpp::as<std::vector<double> >(params);
  Mean_factor_msvolAUX mod(p);

  // instantiate container for filter means
  Rcpp::NumericVector ans(obsTS.rows());

  // create a function to evaluate on particle samples
  std::vector<func> pf_fs;
  pf_fs.emplace_back([](const svec& xt)-> DynMat{ return xt; });

  // iterate through data and save filter means
  ovec yt;
  for (int i = 0; i < obsTS.length(); i++){
    yt << obsTS(i,0), obsTS(i,1);
    mod.filter(yt, pf_fs);
    ans[i] = mod.getExpectations()[0](0,0);
  }
  return ans;
}

