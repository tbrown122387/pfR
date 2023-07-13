#' Build c++ particle filtering code for your R session, and save it as a 
#' pfrmodel object.
#'
#' @param myDir directory with your three code files (i.e. model header, model source and export code)
#' @param model_name your model name. Must be in all lowercase, and be a substring of the above-mentioned filenames
#' @param save if TRUE, then save the compiled object (file name suffix is " _pfrmodel.rds")
#' @return a pfrmodel object 
#' 
#' @examples
#' \dontrun{
#' # compile everything from scratch
#' svol_lev <- buildModelFuncs("~/Desktop", "svol_leverage", save = TRUE)
#' 
#' # ...or load a previously-compiled pfrmodel object
#' svol_lev <- readRDS("~/Desktop/svol_leverage_pfrmodel.rds")
#' 
#' # then use your model's log-likelihood and filtering functions
#' svol_lev_funcs <- unPackFunctions(svol_lev)
#' svol_lev_funcs$svol_leverage_bswc_approx_LL(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' svol_lev_funcs$svol_leverage_bswc_approx_filt(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' }
#' @export
buildModelFuncs <- function(myDir, model_name, save = TRUE){
  
  # get filenames
  allFiles <- list.files(myDir, pattern = model_name, full.names = TRUE)
  headerFile <- allFiles[grepl("\\.h", allFiles)]
  sourceFile <- allFiles[grepl("[^(export)]\\.cpp", allFiles)]
  exportCodeFile <- allFiles[grepl("[(export)]\\.cpp", allFiles)]
  
  # read in files and squash them together
  header_code <- readChar(headerFile, file.info(headerFile)$size)
  source_code <- readChar(sourceFile, file.info(sourceFile)$size)
  export_code <- readChar(exportCodeFile, file.info(exportCodeFile)$size)
  module_code <- r"(
  RCPP_MODULE(mod) { 
  function("%model_name%_bswc_approx_LL", &%model_name%_bswc_approx_LL);  
  function("%model_name%_bswc_approx_filt", &%model_name%_bswc_approx_filt); }
  )"
  module_code <- gsub("%model_name%", model_name, module_code)
  inc <- paste(header_code, source_code, export_code, module_code, sep = "\n")
  
  ### compile c++ code and return (and maybe save) bundled object
  dso <- cxxfunplus::cxxfunctionplus(methods::signature(), 
                                     plugin = "pfr", 
                                     includes = inc, 
                                     save.dso = save)
  obj <- methods::new(
             "pfrmodel",
             model_name = model_name,
             all_code = inc,
             dso = dso, # keep a reference to dso
             header_code = header_code,
             source_code = source_code,
             export_code = export_code)
  
  if( save ){
    rdsFile <- paste0(model_name, "_pfrmodel.rds")
    saveRDS(obj,  file = file.path(myDir, rdsFile))
  } 
  invisible(obj)
  
}


#' Unpack functions from a pfrmodel object.
#'
#' @param pfModel your pfrmodel object
#' @return an Rcpp module with your functions from your export code
#' 
#' @examples
#' \dontrun{
#' # compile everything from scratch
#' svol_lev <- buildModelFuncs("~/Desktop", "svol_leverage", save = TRUE)
#' 
#' # ...or load a previously-compiled pfrmodel object
#' svol_lev <- readRDS("~/Desktop/svol_leverage_pfrmodel.rds")
#' 
#' # then use your model's log-likelihood and filtering functions
#' svol_lev_funcs <- unPackFunctions(svol_lev)
#' svol_lev_funcs$svol_leverage_bswc_approx_LL(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' svol_lev_funcs$svol_leverage_bswc_approx_filt(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' }
#' @export
unPackFunctions <- function(pfModel){
  Rcpp::Module("mod", cxxfunplus::getDynLib(pfModel@dso))
}
