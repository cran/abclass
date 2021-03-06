//
// R package abclass developed by Wenjie Wang <wang@wwenjie.org>
// Copyright (C) 2021-2022 Eli Lilly and Company
//
// This file is part of the R package abclass.
//
// The R package abclass is free software: You can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any later
// version (at your option). See the GNU General Public License at
// <https://www.gnu.org/licenses/> for details.
//
// The R package abclass is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#include <RcppArmadillo.h>
#include <abclass.h>
#include "export-helpers.h"

// [[Rcpp::export]]
Rcpp::List rcpp_boost_group_lasso(
    const arma::mat& x,
    const arma::uvec& y,
    const arma::vec& lambda,
    const unsigned int nlambda,
    const double lambda_min_ratio,
    const arma::vec& group_weight,
    const arma::vec& weight,
    const bool intercept = true,
    const bool standardize = true,
    const unsigned int nfolds = 0,
    const bool stratified_cv = true,
    const unsigned int alignment = 0,
    const unsigned int maxit = 1e5,
    const double epsilon = 1e-3,
    const bool varying_active_set = true,
    const double boost_umin = -5.0,
    const unsigned int verbose = 0
    )
{
    abclass::BoostGroupLasso object {
        x, y, intercept, standardize, weight
    };
    object.set_inner_min(boost_umin);
    return abclass_group_lasso_fit(object,
                                   y,
                                   lambda,
                                   nlambda,
                                   lambda_min_ratio,
                                   group_weight,
                                   nfolds,
                                   stratified_cv,
                                   alignment,
                                   maxit,
                                   epsilon,
                                   varying_active_set,
                                   verbose);
}
