# pfR
pfR is the R interface to [pf](https://github.com/tbrown122387/pf). 

Have a new model you want to quickly write a particle filter for? Quickly generate 99% of the code, fill in your model/algorithm details, and then compile.  

## Step 1: Download

Either clone this repository, or type the following into R:

```
devtools::install_github("tbrown122387/pfr")
```

## Step 2:

Pick your particle filter, generate C++ code templates using one of the following, then fill in the rest:

```
pfr::createAuxiliaryFilterTemplate("my_model")
pfr::createBootstrapFilterWithCovTemplate("my_model")
```

Watch a live demonstration here:



## Step 3:

Recompile your C++ code by hitting <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>L</kbd> or typing

```
devtools::load_all(".")
```

## Step 4:

Run your approximate log-likelihood or filtering functions by typing something like 

```
pfr::my_model_bswc_approx_filt()
pfr::my_model_bswc_approx_LL()
```


