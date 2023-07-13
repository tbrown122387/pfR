# pfR
pfR is the R interface to [pf](https://github.com/tbrown122387/pf). 

Have a new model you want to quickly write a particle filter for? Quickly generate 99% of the code, fill in your model/algorithm details, and then compile.  

## Step 1: Download

Either clone this repository, or type the following into R:

```
devtools::install_github("tbrown122387/pfr")
```

## Step 2: Auto-generate code

Pick your particle filter, generate most of the required `c++` code, then fill in the rest. For example, 

```
library(pfr)
createPFCPPTemplates("svol_leverage", "BSWC", fileDir = "~/Desktop/")
```

Saves three files to your desktop: `svol_leverage_bswc.h`, `svol_leverage_bswc.cpp`, and `svol_leverage_bswc_export.cpp`. After you're finished filling in the details, they should look like TODO.

## Step 3: Compile and Load

Compile the `c++` code with 

```
svol_lev <- buildModelFuncs("~/Desktop", "svol_leverage")
```

or load a previously-compiled object with 

```
svol_lev <- readRDS("~/Desktop/svol_leverage_pfrmodel.rds")
```

## Step 4: Run

Unpack your model's functions as an `Rcpp` Module object, and call them

```
svol_lev_funcs <- unPackFunctions(svol_lev)
svol_lev_funcs$svol_leverage_bswc_approx_LL(rnorm(100), c(.9, 0.0, 1.0, -.2))
svol_lev_funcs$svol_leverage_bswc_approx_filt(rnorm(100), c(.9, 0.0, 1.0, -.2))
```


