#include "svol_leverage.h" 

// Expose the svol leverage model class
// Recall FLOATTYPE is defined in the header we're including above
RCPP_MODULE(svol_leverage_module){
  Rcpp::class_<svol_leverage>("svol_leverage")
  .constructor<FLOATTYPE, FLOATTYPE, FLOATTYPE, FLOATTYPE>()
  .method("getLogCondLike", &svol_leverage::getLogCondLike);
  //.method("lastLogCondLike", &svol_leverage::lastLogCondLike); 

}