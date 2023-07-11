##
## Define pfr plugin for inline package.
##

inc_path_fun <- function(package) {
  system.file('include', package = package)
}


# # Using RcppEigen
eigen_path_fun <- function() {
  system.file('include', package = 'RcppEigen')
}

PKG_CPPFLAGS_env_fun <- function() {

  paste(' -I"', file.path(inc_path_fun("Rcpp"), '" '),
        ' -I"', file.path(eigen_path_fun(), '" '),
        ' -I"', file.path(eigen_path_fun(), 'unsupported" '),
        ' -DEIGEN_NO_DEBUG ',
        ' -DDROPPINGTHISINRPACKAGE ',
        # ' -include ', shQuote(Eigen), ' ',
        # ifelse (.Platform$OS.type == "windows", ' -std=c++1y',
        #         ' -D_REENTRANT -DRCPP_PARALLEL_USE_TBB=1 '),
        sep = '')
}

legitimate_space_in_path <- function(path) {
  # For windows, use the short path name (8.3 format)
  #
  if (.Platform$OS.type == "windows") {
    path <- normalizePath(path)
    if (grepl(" ", path, fixed = TRUE))
      path <- utils::shortPathName(path)
    # it is weird that the '\\' in the path name will be gone
    # when passed to cxxfunction, so change it to '/'
    path <- gsub('\\\\', '/', path, perl = TRUE)
  }
  path
}

pfrplugin <- function() {
  Rcpp_plugin <- inline::getPlugin("Rcpp")
  rcpp_pkg_libs <- Rcpp_plugin$env$PKG_LIBS
  rcpp_pkg_path <- system.file(package = 'Rcpp')
  rcpp_pkg_path2 <- legitimate_space_in_path(rcpp_pkg_path)
  
  # If rcpp_PKG_LIBS contains space without preceding '\\', add `\\';
  # otherwise keept it intact
  if (grepl('[^\\\\]\\s', rcpp_pkg_libs, perl = TRUE))
    rcpp_pkg_libs <- gsub(rcpp_pkg_path, rcpp_pkg_path2, rcpp_pkg_libs, fixed = TRUE)
  
  if (.Platform$OS.type == "windows") {
    list(includes = '// [[Rcpp::plugins(cpp14)]]\n',
         body = function(x) x,
         env = list(PKG_CPPFLAGS = paste(Rcpp_plugin$env$PKG_CPPFLAGS,
                                         PKG_CPPFLAGS_env_fun(), collapse = " ")))
  } else {
    list(includes = '// [[Rcpp::plugins(cpp14)]]\n',
         body = function(x) x,
         env = list(PKG_CPPFLAGS = paste(Rcpp_plugin$env$PKG_CPPFLAGS,
                                         PKG_CPPFLAGS_env_fun(), collapse = " ")))
  }
}


# inlineCxxPlugin would automatically get registered in inline's plugin list.
# Note that everytime pfr plugin is used, inlineCxxPlugin
# gets called so we can change some settings on the fly
# for example now by setting (TODO rstan_options(boost_lib=xxx))
inlineCxxPlugin <- function(...) {
  settings <- pfrplugin()
  settings
}

