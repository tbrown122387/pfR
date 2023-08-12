#' Create c++ template files for bootstrap filters (with or without covariates), 
#' auxiliary particle filters, sequential importance sampling with resampling 
#' filters, or Rao-Blackwellized/Marginal particle filters. 
#'
#' @param modname name of model in all lowercase
#' @param pfAlgo Either "BSF", "APF", "BSWC", "SISR", "RBPFHMM", or "RBPFKALMAN"
#' @param fileDir where to save files. Not saved if NULL (but three files are returned in list).
#' @param openNow TRUE if you want to open this now in RStudio. Ignored if fileDir is NULL.
#' @return NULL if saving files, otherwise a list with three character vectors 
#' 
#' @examples
#' # return in list of character strings
#' createPFCPPTemplates("coolmod", "BSF", fileDir = NULL)
#' 
#' \dontrun{
#' # save three files to Desktop, and
#' # begin editing them in rstudio IDE
#' createPFCPPTemplates("coolmod", "BSF", fileDir = "~/Desktop/")
#' }
#' @export
createPFCPPTemplates <- function(modname, pfAlgo, fileDir, openNow = TRUE){
  
  # check arguments
  validName <- nchar(modname) > 0 & !grepl("[^a-z_]", modname)
  validPath <- ifelse(is.null(fileDir), TRUE, dir.exists(fileDir))
  validAlgo <- pfAlgo %in% algoTypes
  stopifnot(validName)
  stopifnot(validPath)
  stopifnot(validAlgo)
  
  # create names to swap into templates
  headerGuardName <- paste0(toupper(modname), "_", pfAlgo, "_H")
  capsName <- paste0("_", toupper(modname), "_", pfAlgo)
  lowerAlgoAbbrev <- tolower(pfAlgo)
  className <- paste0(toupper(substr(modname, 1, 1)), 
                      substr(modname, 2, nchar(modname)), "_",
                      lowerAlgoAbbrev)
  headerFilename <- paste0(modname, "_", lowerAlgoAbbrev, ".h")
  sourceFilename <- paste0(modname, "_", lowerAlgoAbbrev, ".cpp")
  exportFilename <- paste0(modname, "_", lowerAlgoAbbrev, "_export.cpp")
  aproxLLName <- paste0(modname, "_", lowerAlgoAbbrev, "_approx_LL")
  aproxFiltName <- paste0(modname, "_", lowerAlgoAbbrev, "_approx_filt")
  
  # select algo
  if(pfAlgo == "BSWC"){
    header_template <- cpptemp_bswc_header
    source_template <- cpptemp_bswc_source
    export_template <- cpptemp_bswc_export
  }else if( pfAlgo == "APF"){
    header_template <- cpptemp_auxpf_header
    source_template <- cpptemp_auxpf_source
    export_template <- cpptemp_auxpf_export
  }else if( pfAlgo == "BSF") {
    header_template <- cpptemp_bsfilter_header
    source_template <- cpptemp_bsfilter_source
    export_template <- cpptemp_bsfilter_export
  }else if( pfAlgo == "SISR"){
    header_template <- cpptemp_sisr_header
    source_template <- cpptemp_sisr_source
    export_template <- cpptemp_sisr_export
  }else if( pfAlgo == "RBPFHMM"){
    header_template <- cpptemp_rbpfhmm_header
    source_template <- cpptemp_rbpfhmm_source
    export_template <- cpptemp_rbpfhmm_export
  }else if( pfAlgo == "RBPFKALMAN"){
    header_template <- cpptemp_rbpfkalman_header
    source_template <- cpptemp_rbpfkalman_source
    export_template <- cpptemp_rbpfkalman_export
  }

  ###### 1 of 3: the model/algo header file 
  # swap names into template
  fStrHeader <- gsub(pattern = "HEADERGUARDMODNAME_H", 
                     replacement = headerGuardName, 
                     x = header_template)
  fStrHeader <- gsub(pattern = "_MODNAME", 
                     replacement = capsName, 
                     x = fStrHeader)
  fStrHeader <- gsub(pattern = "classMODNAME", 
                     replacement = className, 
                     x = fStrHeader)
  
  ###### 2 of 3: the model/algo source file 
  # swap names into template
  fStrSource <- gsub(pattern = "filenameMODNAME.h", 
                     replacement = headerFilename, 
                     x = source_template)
  fStrSource <- gsub(pattern = "classMODNAME", 
                     replacement = className, 
                     x = fStrSource)
  
  ###### 3 of 3: the export file
  # swap names into template
  fStrExport <- gsub(pattern = "filenameMODNAME.h", 
                     replacement = headerFilename, 
                     x = export_template)
  fStrExport <- gsub(pattern = "modname_approx_LL", 
                     replacement = aproxLLName, 
                     x = fStrExport)
  fStrExport <- gsub(pattern = "modname_approx_filt", 
                     replacement = aproxFiltName, 
                     x = fStrExport)
  fStrExport <- gsub(pattern = "classMODNAME", 
                     replacement = className, 
                     x = fStrExport)
  
  # check for previously-existing code to make sure you don't overwrite it
  headerFilePath <- file.path(fileDir, headerFilename)
  sourceFilePath <- file.path(fileDir, sourceFilename)
  exportCodeFilePath <- file.path(fileDir, exportFilename)
  
  if ( any(file.exists(c(headerFilePath, sourceFilePath, exportCodeFilePath))) ){
    stop("C++ code for this model already exists.")
  }  
  
  # potentially save file and open up file in rstudio
  if( !is.null(fileDir) ){
    
    # write header file (1 of 3)
    file.create(headerFilePath)
    fileConn1 <- file(headerFilePath)
    writeLines(fStrHeader, fileConn1)
    close(fileConn1)
    
    # write source file (2 of 3)
    file.create(sourceFilePath)
    fileConn2 <- file(sourceFilePath)
    writeLines(fStrSource, fileConn2)
    close(fileConn2)
    
    # write source file (3 of 3)    
    file.create(exportCodeFilePath)
    fileConn3 <- file(exportCodeFilePath)
    writeLines(fStrExport, fileConn3)
    close(fileConn3)
    
    if( openNow ){
      rstudioapi::navigateToFile(headerFilePath)
      rstudioapi::navigateToFile(sourceFilePath)
      rstudioapi::navigateToFile(exportCodeFilePath)
    }
  }else{
    
    # not saving files, and at the end of the function
    # so just spit out a big list of three content strings
    return(
      list(
        export_code = fStrExport,
        header_code = fStrHeader, 
        source_code = fStrSource
      ))
  }
}



algoTypes <- c("BSWC", "APF","BSF","SISR","RBPFHMM", "RBPFKALMAN")

