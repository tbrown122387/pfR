#' Build c++ particle filtering code for your R session.
#'
#' @param myDir directory with your three code files (i.e. model header, model source and export code)
#' @param modelName your model name. Must be in all lowercase, and be a substring of the above-mentioned filenames
#' @param verbose logical and passed in to inline::cxxfunction()
#' @return an Rcpp Module object 
#' 
#' @examples
#' \dontrun{
#' # compile everything from scratch
#' svol_lev <- buildModelFuncs("~/Desktop", "svol_leverage")
#' 
#' # then use your model's log-likelihood and filtering functions
#' svol_lev$svol_leverage_bswc_approx_LL(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' svol_lev$svol_leverage_bswc_approx_filt(rnorm(100), c(.9, 0.0, 1.0, -.2))
#' }
#' @export
buildModelFuncs <- function(myDir, modelName, verbose = FALSE){
  
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
  RCPP_MODULE(%model_name%_%algo_name%_mod) { 
    function("%model_name%_%algo_name%_approx_LL", &%model_name%_%algo_name%_approx_LL);  
    function("%model_name%_%algo_name%_approx_filt", &%model_name%_%algo_name%_approx_filt); }
  )"
  module_code <- gsub(pattern = "%model_name%", 
                      replacement = modelName, 
                      x = module_code)
  module_code <- gsub("%algo_name%", algoName, module_code)
  inc <- paste(header_code, 
               source_code, 
               export_code, 
               module_code, sep = "\n")
  
  ### compile c++ code and return (and maybe save) bundled object
  fx <- inline::cxxfunction(methods::signature(), 
                       plugin = "pfr", 
                       includes = inc, verbose = verbose)
  moduleName <- paste(modelName, algoName, "mod", sep = "_") 
  mod <- Rcpp::Module(moduleName, inline::getDynLib(fx))
  invisible(mod)
}


