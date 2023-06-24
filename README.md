# pfR
pfR is the R interface to [pf](https://github.com/tbrown122387/pf). 

Have a new model you want to quickly write a particle filter for? Quickly generate 99% of the code, fill in your model/algorithm details, and then compile.  

## Step 1: Download

Either clone this repository, or type the following into R:

```
devtools::install_github("tbrown122387/pfr")
```

## Step 2: Auto-generate code

Pick your particle filter, generate C++ code templates using one of the following, then fill in the rest:

```
pfr::createSISRFilterTemplate("my_model")
pfr::createAuxiliaryFilterTemplate("my_model")
pfr::createBootstrapFilterTemplate("my_model")
pfr::createBootstrapFilterWithCovTemplate("my_model")
pfr::createRBPFHmmTemplate("my_model")
pfr::createRBPFKalmanTemplate("my_model")
```

Doubting your C++ skills? Check out the full examples [here](src/) or watch this live demonstration:

https://github.com/tbrown122387/pfR/assets/1740324/99447477-5ca1-4e1e-9dba-7417133aa932


## Step 3: Compile and Load

Recompile your C++ code by hitting <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>L</kbd> or typing

```
devtools::load_all(".")
```

## Step 4: Run

Run your approximate log-likelihood and/or filtering functions by typing something like 

```
pfr::my_model_bswc_approx_filt(myData, myParams)
pfr::my_model_bswc_approx_LL(myData, myParams)
```

The name of these functions will depend on what you named your model/algorithm.
