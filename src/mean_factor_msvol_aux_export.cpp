// // replace every instance of <TODO> with your own code! 
// 
// #include "mean_factor_msvol_aux.h" 
// 
// //' @useDynLib pfr, .registration = TRUE
// //' @import RcppEigen
// //' @importFrom Rcpp evalCpp
// //' @export
// // [[Rcpp::export]]
// double mean_factor_msvol_aux_approx_LL(const Rcpp::NumericMatrix& obsTS, const Eigen::VectorXd& params){
// 
//   // instantiate model with arg params
//   // std::vector<double> p = Rcpp::as<std::vector<double> >(params);
//   Mean_factor_msvolAUX mod(params);
// 
//   // calculate likelihood by iterating through time series
//   double ans(0.0);
//   ovec yt;
//   for (int i = 0; i < obsTS.rows(); i++){
//     yt << obsTS(i,0), obsTS(i,1);
//     mod.filter(yt);
//     ans += mod.getLogCondLike();
//   }
//   return ans;
// }
// 
// 
// //' @useDynLib pfr, .registration = TRUE
// //' @import RcppEigen
// //' @importFrom Rcpp evalCpp
// //' @export
// // [[Rcpp::export]]
// Eigen::MatrixXd mean_factor_msvol_aux_approx_filt(
//     const Rcpp::NumericMatrix& obsTS,
//     const Eigen::VectorXd& params){
//   
//   // instantiate model with arg params
//   // std::vector<double> p = Rcpp::as<std::vector<double> >(params);
//   Mean_factor_msvolAUX mod(params);
// 
//   // instantiate container for filter means
//   Eigen::MatrixXd ans(obsTS.rows(), dimstate_MEAN_FACTOR_MSVOL_AUX);
// 
//   // create a function to evaluate on particle samples
//   std::vector<func> pf_fs;
//   pf_fs.emplace_back([](const svec& xt)-> DynMat{ return xt; });
// 
//   // iterate through data and save filter means
//   ovec yt;
//   for (int i = 0; i < obsTS.rows(); i++){
//     yt << obsTS(i,0), obsTS(i,1);
//     mod.filter(yt, pf_fs);
//     ans.row(i) = mod.getExpectations()[0].col(0).transpose();
//   }
//   return ans;
// }
// 
