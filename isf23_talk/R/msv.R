# load package 
devtools::load_all("~/pfr")

# get some data
tickers <- c("SPY","QQQ")
quantmod::getSymbols(tickers, from = Sys.Date() - 252)
rets <- lapply(tickers, \(x) { 100*diff(log(quantmod::Ad(get(x)))) } )
rets <- as.matrix(as.data.frame(rets)[-1,])

#########################################
# Particle Marginal Metropolis-Hastings #
#########################################

# prior motiv. from Kim, S., N. Shephard, and S. Chib (1998) 
logUnnormPrior <- function(paramVec){
  Bpart <- dnorm(paramVec[1], mean = 1, sd = 1, log = T)
  MuPart <- sum(dnorm(paramVec[2:4], 0, 1, log = T))
  PhiPart <- sum(dbeta(.5*(paramVec[5:7]+1), shape1 = 20, shape2 = 1.5, log = T)) 
  SigmaPart <- sum(dgamma(1/paramVec[8:10]^2, shape = 5/2, scale = .05/2, log = T) -3*log(paramVec[8:10]))
  Bpart + MuPart + PhiPart + SigmaPart
}

# performs one MCMC iteration
iterOnce <- function(old, disp = .01){
  
  proposal <- old$theta + rnorm(n = length(old$theta) )*disp
  newLogPrior <- logUnnormPrior(proposal)
  #cat(newLogPrior, "\n")
  if( is.infinite(newLogPrior) | is.nan(newLogPrior))
    return(old)
  
  newLL <- pfr::mean_factor_msvol_aux_approx_LL(obsTS = rets, params = proposal)
  
  if( log(runif(1)) < newLL + newLogPrior - old$approxLL - old$logPrior){
    return(list(theta=proposal, approxLL=newLL, logPrior= newLogPrior))
  }else{
    return(old)
  }
}


# Begin Sampling

numSamps <- 50000 
samples <- vector(mode = "list", length = numSamps)

# Option A: set initial guess and do the sampling

# start off with arbitrary guess
# startParams <- c(1, rep(0,3), rep(.9, 3), rep(.01, 3))
# samples[[1]] <- list(theta=startParams, 
#                      approxLL=pfr::mean_factor_msvol_aux_approx_LL(obsTS = rets, 
#                                                                    params = startParams), 
#                      logPrior= logUnnormPrior(startParams))

# Option B: or start where you left off from last time
samples[[1]] <- list(theta=c(1.2408866, -1.4210863,  0.5557985,  0.3790061,  0.8986639,  0.8311591,  0.9191384,  3.5923944,  4.4163517,  4.9755177),
                     approxLL=-487.8882,
                     logPrior= -4.987007)

# Do all the work
for(i in 2:numSamps){
  samples[[i]] <- iterOnce(samples[[i-1]])
}

# save the work to disk
save.image(file = "msvol_samples.RData")



###################
##combine samps
######################
load("msvol_samples_1of2.RData")
sampMat <- t(sapply(samples, '[[', 1))
sampMat <- as.data.frame(sampMat)
colnames(sampMat) <- c("Bqqq",
                       "Muspy", "Muqqq","MuD",
                       "Phispy", "Phiqqq","PhiD",
                       "Sigmaspy", "Sigmaqqq","SigmaD")
s <- sampMat
load("msvol_samples_2of2.RData")
sampMat <- t(sapply(samples, '[[', 1))
sampMat <- as.data.frame(sampMat)
colnames(sampMat) <- c("Bqqq",
                       "Muspy", "Muqqq","MuD",
                       "Phispy", "Phiqqq","PhiD",
                       "Sigmaspy", "Sigmaqqq","SigmaD")

s <- rbind(s, sampMat)
load("msvol_samples_3of3.RData")
sampMat <- t(sapply(samples, '[[', 1))
sampMat <- as.data.frame(sampMat)
colnames(sampMat) <- c("Bqqq",
                       "Muspy", "Muqqq","MuD",
                       "Phispy", "Phiqqq","PhiD",
                       "Sigmaspy", "Sigmaqqq","SigmaD")

s <- rbind(s, sampMat)
load("msvol_samples_4.RData")
sampMat <- t(sapply(samples, '[[', 1))
sampMat <- as.data.frame(sampMat)
colnames(sampMat) <- c("Bqqq",
                       "Muspy", "Muqqq","MuD",
                       "Phispy", "Phiqqq","PhiD",
                       "Sigmaspy", "Sigmaqqq","SigmaD")

s <- rbind(s, sampMat)



# use param estimates to filter
estPar <- colMeans(s)
pfr::mean_factor_msvol_aux_approx_LL(obsTS = rets, params = estPar)
