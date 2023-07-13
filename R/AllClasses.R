#' Class to hold compiled code. 
#'
#'@importClassesFrom cxxfunplus cxxdso
setClass("pfrmodel",
         representation = representation(
           model_name = "character",
           all_code = "character",
           dso = "cxxdso",
           header_code = "character",
           source_code = "character",
           export_code = "character"))
