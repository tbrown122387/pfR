## R CMD check results

0 errors | 0 warnings | 2 notes


Both notes seem necessary. This package builds c++ code during an interactive R session, but does not contain any source c++ in itself as a package. `LinkingTo` is the appropriate field for the `DESCRIPTION` file, not `Imports`, but if If I remove BH and RcppEigen from the `Imports` field, the code examples in the vignette will fail to build.


❯ checking dependencies in R code ... NOTE
  Namespaces in Imports field not imported from:
    ‘BH’ ‘RcppEigen’
    All declared Imports should be used.

❯ checking DESCRIPTION meta-information ... NOTE
  'LinkingTo' field is unused: package has no 'src' directory

