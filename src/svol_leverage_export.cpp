#include "svol_leverage.h" 

// [[Rcpp::export]]
double svol_leverage_approx_LL(const Rcpp::NumericVector& obsTS, 
                       const Rcpp::NumericVector& params){
  // param order: phi, mu, sigma, rho
  svol_leverage mod(params[0], params[1], params[2], params[3]);
  double ans(0.0);
  vec yt, ytm1;
  for (int i = 1; i < obsTS.length(); i++){
    yt << obsTS[i]; ytm1 << obsTS[i-1];
    mod.filter(yt, ytm1);
    ans += mod.getLogCondLike();
  }
  return ans;
}

/*** R
pfr::svol_leverage_approx_LL(rnorm(100), c(.9, 0, 1, .5))
*/



// [[Rcpp::export]]
Rcpp::NumericVector svol_leverage_approx_filt(
    const Rcpp::NumericVector& obsTS,
    const Rcpp::NumericVector& params){
  
  // param order: phi, mu, sigma, rho
  svol_leverage mod(params[0], params[1], params[2], params[3]);
  Rcpp::NumericVector ans(obsTS.size() - 1);

  // create a function to evaluate on particle samples
  std::vector<std::function<const DynMat(const vec&, const vec&)>> pf_fs;
  pf_fs.emplace_back([](const vec& xt, const vec& zt)-> DynMat{ return xt; });

  // iterate through data and save filter means
  vec yt, ytm1;
  for (int i = 1; i < obsTS.length(); i++){
    yt << obsTS[i]; ytm1 << obsTS[i-1];
    mod.filter(yt, ytm1, pf_fs);
    ans[i-1] = mod.getExpectations()[0](0,0);
  }
  return ans;
}


/*** R
pfr::svol_leverage_approx_filt(rnorm(100), c(.9, 0, 1, .5))
*/