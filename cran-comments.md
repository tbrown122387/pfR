## R CMD check results

0 errors | 0 warnings | 0 notes

Regarding your comment "\dontrun{} should only be used if the example really cannot be executed (e.g. because of missing additional software, missing API keys, ...) by the user. ":

 - The second example in createPFCPPTemplates()'s (R/template_creators.R) documentation cannot be run because it relies on RStudio. 

Regarding your comment "Please unwrap the examples if they are executable in < 5 sec,"

 - The examples in buildModelFuncs() (R/compile.R) cannot be run because they take longer than 5 seconds to compile the c++ code.

Regarding your comment "Please ensure that your functions do not write by default"

 - fixed.

Regarding "or in your examples/vignettes/tests in the user's home filespace"

 - They don't. The sample code in the vignette appears to, but that R code chunk is not evaluated. 

