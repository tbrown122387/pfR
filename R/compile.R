#' Build c++ particle filtering code for your R session, and save it as a 
#' pfrmodel object.
#'
#' @param myDir directory with your three code files (i.e. model header, model source and export code)
#' @param modelName your model name. Must be in all lowercase, and be a substring of the above-mentioned filenames
#' @param save if TRUE, then save the compiled object (file name suffix is " _pfrmodel.rds")
#' @return a pfrmodel object 
#' 
#' @examples
#' \dontrun{
#' # compile everything from scratch
#' svol_lev <- buildModelFuncs("~/Desktop", "svol_leverage", save = TRUE)
#' 
#' # or load a previously-compiled pfrmodel object
#' svol_lev <- readRDS("~/Desktop/svol_leverage_pfrmodel.rds")
#' 
#' # then use your model's log-likelihood and filtering functions
#' svol_lev_funcs <- unPackFunctions(svol_lev)
#' svol_lev_funcs$svol_leverage_bswc_approx_LL(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' svol_lev_funcs$svol_leverage_bswc_approx_filt(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' }
#' @export
buildModelFuncs <- function(myDir, modelName, save = TRUE){
  
  # get filenames and algo type
  allFiles <- list.files(myDir, pattern = modelName, full.names = TRUE)
  headerFile <- allFiles[grepl("\\.h", allFiles)]
  sourceFile <- allFiles[grepl("[^(export)]\\.cpp", allFiles)]
  exportCodeFile <- allFiles[grepl("[(export)]\\.cpp", allFiles)]
  regex <- paste0("(", paste(tolower(algoTypes), collapse = "|"), ")")
  featured_algos <- regmatches(x = allFiles, 
                               m = regexpr(pattern = regex, text = allFiles))
  algoName <- unique(featured_algos)
  stopifnot(length(algoName) == 1 & toupper(algoName) %in% algoTypes)
  
  
  # read in files and squash them together
  header_code <- readChar(headerFile, file.info(headerFile)$size)
  source_code <- readChar(sourceFile, file.info(sourceFile)$size)
  export_code <- readChar(exportCodeFile, file.info(exportCodeFile)$size)
  module_code <- r"(
  RCPP_MODULE(mod) { 
  function("%model_name%_%algo_name%_approx_LL", &%model_name%_%algo_name%_approx_LL);  
  function("%model_name%_%algo_name%_approx_filt", &%model_name%_%algo_name%_approx_filt); }
  )"
  module_code <- gsub(pattern = "%model_name%", 
                      replacement = modelName, 
                      x = module_code)
  module_code <- gsub("%algo_name%", algoName, module_code)
  inc <- paste(header_code, source_code, export_code, module_code, sep = "\n")
  
  ### compile c++ code and return (and maybe save) bundled object
  # inline::cxxfunction(methods::signature(), 
  #                     plugin = "pfr", 
  #                     includes = inc, verbose = TRUE)
  dso <- cxxfunplus::cxxfunctionplus(methods::signature(), 
                                     plugin = "pfr", 
                                     includes = inc, 
                                     save.dso = save)#, verbose = TRUE)
  obj <- methods::new(
             "pfrmodel",
             model_name = modelName,
             all_code = inc,
             dso = dso, # keep a reference to dso
             header_code = header_code,
             source_code = source_code,
             export_code = export_code)
  
  if( save ){
    rdsFile <- paste0(modelName, "_pfrmodel.rds")
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
