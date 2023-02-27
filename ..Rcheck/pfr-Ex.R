pkgname <- "pfr"
source(file.path(R.home("share"), "R", "examples-header.R"))
options(warn = 1)
library('pfr')

base::assign(".oldSearch", base::search(), pos = 'CheckExEnv')
base::assign(".old_wd", base::getwd(), pos = 'CheckExEnv')
cleanEx()
nameEx("pfr-package")
### * pfr-package

flush(stderr()); flush(stdout())

### Name: pfr-package
### Title: What the package does (short line)
### Aliases: pfr-package pfr
### Keywords: package

### ** Examples

~~ simple examples of the most important functions ~~



cleanEx()
nameEx("rcppeigen_hello_world")
### * rcppeigen_hello_world

flush(stderr()); flush(stdout())

### Name: RcppEigen-Functions
### Title: Set of functions in example RcppEigen package
### Aliases: rcppeigen_hello_world rcppeigen_innerproduct
###   rcppeigen_outerproduct rcppeigen_bothproducts

### ** Examples

  x <- sqrt(1:4)
  rcppeigen_innerproduct(x)
  rcppeigen_outerproduct(x)



### * <FOOTER>
###
cleanEx()
options(digits = 7L)
base::cat("Time elapsed: ", proc.time() - base::get("ptime", pos = 'CheckExEnv'),"\n")
grDevices::dev.off()
###
### Local variables: ***
### mode: outline-minor ***
### outline-regexp: "\\(> \\)?### [*]+" ***
### End: ***
quit('no')
