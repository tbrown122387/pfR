#' Create file templates for a bootstrap filter model (with covariates).
#' 
#' If you want to run a bootstrap filter with covariates, this function creates 
#' the three required C++ files associated with your state-space model.
#'
#' @param modname name of model in all lowercase
#' @param saveFile TRUE if you want to save the file, FALSE otherwise
#' @param fileDir where to save files. If NULL, saves to ./src/ 
#'
#' @return NULL if saving a file, otherwise a list with three character vectors 
#'
#' @examples
#' # files pop up in rstudio
#' createBootstrapFilterWithCovTemplate("coolmod") 
#' 
#' # does not pop up in rstudio
#' myCon <- file("myFile.cpp", "wb")
#' threeCodes <- createBootstrapFilterWithCovTemplate("mod", saveFile = F)
#' expCodeStr <- threeCodes$export_code
#' writeLines(expCodeStr , myCon )
#' close(myCon)
#'
#' @export
createBootstrapFilterWithCovTemplate <- function(modname, saveFile = TRUE, fileDir = NULL){
  
  # check arguments
  validName <- nchar(modname) > 0 & !grepl("[^a-z_]", modname)
  validPath <- ifelse(is.null(fileDir), TRUE, dir.exists(fileDir))
  stopifnot(validName)
  stopifnot(validPath)
  
  # create names to swap into templates
  headerGuardName <- paste0(toupper(modname), "_BSWC_H")
  capsName <- paste0("_", toupper(modname), "_BSWC")
  className <- paste0(toupper(substr(modname, 1, 1)), 
                      substr(modname, 2, nchar(modname)),
                      "BSWC")
  headerFilename <- paste0(modname, "_bswc.h")
  sourceFilename <- paste0(modname, "_bswc.cpp")
  exportFilename <- paste0(modname, "_bswc_export.cpp")
  aproxLLName <- paste0(modname, "_bswc_approx_LL")
  aproxFiltName <- paste0(modname, "_bswc_approx_filt")
  
  ###### 1 of 3: the model/algo header file 
  # swap names into template
  fStrHeader <- gsub(pattern = "HEADERGUARDMODNAME_H", 
                     replacement = headerGuardName, 
                     x = cpptemp_bswc_header)
  fStrHeader <- gsub(pattern = "_MODNAME", 
                     replacement = capsName, 
                     x = fStrHeader)
  fStrHeader <- gsub(pattern = "classMODNAME", 
                     replacement = className, 
                     x = fStrHeader)
  
  # save file and open it up in rstudio
  if( is.null(fileDir)){
    filePath1 <- file.path(getwd(), "src", headerFilename)
  }else{
    filePath1 <- file.path(fileDir, headerFilename)
  }
  
  if( saveFile ){
    file.create(filePath1)
    fileConn1 <- file(filePath1)
    writeLines(fStrHeader, fileConn1)
    close(fileConn1)
    rstudioapi::navigateToFile(filePath1)
  }
  
  ###### 2 of 3: the model/algo source file 
  # swap names into template
  fStrSource <- gsub(pattern = "filenameMODNAME.h", 
                     replacement = headerFilename, 
                     x = cpptemp_bswc_source)
  fStrSource <- gsub(pattern = "classMODNAME", 
                     replacement = className, 
                     x = fStrSource)

  
  # save file and open it up in rstudio
  if( is.null(fileDir)){
    filePath2 <- file.path(getwd(), "src", sourceFilename)
  }else{
    filePath2 <- file.path(fileDir, sourceFilename)
  }
  
  if( saveFile ){
    file.create(filePath2)
    fileConn2 <- file(filePath2)
    writeLines(fStrSource, fileConn2)
    close(fileConn2)
    rstudioapi::navigateToFile(filePath2)
  }
  
  ###### 3 of 3: the export file
  # swap names into template
  fStrExport <- gsub(pattern = "filenameMODNAME.h", 
                     replacement = headerFilename, 
                     x = cpptemp_bswc_export)
  fStrExport <- gsub(pattern = "modname_approx_LL", 
                     replacement = aproxLLName, 
                     x = fStrExport)
  fStrExport <- gsub(pattern = "modname_approx_filt", 
                     replacement = aproxFiltName, 
                     x = fStrExport)
  fStrExport <- gsub(pattern = "classMODNAME", 
                     replacement = className, 
                     x = fStrExport)
  
  
  # save file and open it up in rstudio
  if( is.null(fileDir)){
    filePath3 <- file.path(getwd(), "src", exportFilename)
  }else{
    filePath3 <- file.path(fileDir, exportFilename)
  }
  
  if( saveFile ){
    file.create(filePath3)
    fileConn3 <- file(filePath3)
    writeLines(fStrExport, fileConn3)
    close(fileConn3)
    rstudioapi::navigateToFile(filePath3)
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
