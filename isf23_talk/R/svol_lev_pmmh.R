# load package 
devtools::load_all("~/pfr")

# get some data
spy <- quantmod::getSymbols("SPY", 
                            auto.assign = F, 
                            from = Sys.Date() - 252)
rets <- 100*diff(log(spy$SPY.Adjusted))

#########################################
# Particle Marginal Metropolis-Hastings #
#########################################

# prior from Kim, S., N. Shephard, and S. Chib (1998)
logUnnormPrior <- function(paramVec){
  phiPart <- dbeta(.5*(paramVec[1]+1), shape1 = 20, shape2 = 1.5, log = T)
  muPart <- dnorm(paramVec[2], 0, 1, log = T)
  sigmaPart <- dgamma(1/paramVec[3]^2, shape = 5/2, scale = .05/2, log = T) -3*log(paramVec[3])
  rhoPart <- dunif(paramVec[4], min = -1, max = 1, log = T)
  phiPart + muPart + sigmaPart + rhoPart
}

# performs one MCMC iteration
iterOnce <- function(old, disp = .01){
  
  proposal <- old$theta + rnorm(n = length(old$theta) )*disp
  newLogPrior <- logUnnormPrior(proposal)
  
  if( is.infinite(newLogPrior) | is.nan(newLogPrior))
    return(old)

  newLL <- pfr::svol_leverage_bswc_approx_LL(obsTS = rets, 
                                             params = proposal)
  if( log(runif(1)) < newLL + newLogPrior - old$approxLL - old$logPrior){
    return(list(theta=proposal, approxLL=newLL, logPrior= newLogPrior))
  }else{
    return(old)
  }
}

# set initial guess and do the sampling
numSamps <- 5e4
samples <- vector(mode = "list", length = numSamps)
startParams <- c(.9, 0, .4, -.1)
samples[[1]] <- list(theta=startParams, 
                     approxLL=pfr::svol_leverage_bswc_approx_LL(obsTS = rets, params = startParams), 
                     logPrior= logUnnormPrior(startParams))
for(i in 2:numSamps){
  samples[[i]] <- iterOnce(samples[[i-1]])
}

# save the image once you're done
save.image(file = "svol_lev_samples.RData")

# # vis
# load("~/isf23_talk/R/svol_lev_samples.RData")
# sampMat <- as.data.frame(t(sapply(samples, '[[', 1)))
# # plot.ts(t(sapply(samples, '[[', 1)))
# plot.ts(sapply(sampMat, cumsum) / 1:nrow(sampMat))
# colMeans(sampMat)
