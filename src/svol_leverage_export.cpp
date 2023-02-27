#include "svol_leverage.h" 

using BasePF = BSFilterWC<nparts_svol_lev, dimstate, dimobs, dimobs, resampT, FLOATTYPE>;

BasePF *newSvolLeverage(FLOATTYPE phi, FLOATTYPE mu, FLOATTYPE sigma, FLOATTYPE rho) {
  return new svol_leverage(phi, mu, sigma, rho);
}


// Expose the svol leverage model class
// Recall FLOATTYPE is defined in the header we're including above
RCPP_MODULE(svol_leverage_module){
  
  Rcpp::class_< BasePF >("BasePF")
  .factory<FLOATTYPE,FLOATTYPE,FLOATTYPE,FLOATTYPE>(newSvolLeverage)
  .method("getLogCondLike", &BasePF::getLogCondLike);
  
  // 
  // Rcpp::class_<>("BasePF")
  // .method("getLogCondLike", &BSFilterWC<nparts_svol_lev, dimstate, dimobs, dimobs, resampT, FLOATTYPE>::getLogCondLike);
  //.default_constructor()
  
  //.constructor<const unsigned int &>()
  
//  .field("x", &Base::x) 

  // Rcpp::class_<svol_leverage>("svol_leverage")
  // .derives<BSFilterWC<nparts_svol_lev, dimstate, dimobs, dimobs, resampT, FLOATTYPE>>("BasePF")
  // .constructor<FLOATTYPE, FLOATTYPE, FLOATTYPE, FLOATTYPE>();
  // .method("lol", &svol_leverage::lol)
  // .method("lastLogCondLike", &svol_leverage::lastLogCondLike); 
  //.method("getLogCondLike", &svol_leverage::getLogCondLike);
}