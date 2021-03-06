#include <RcppArmadillo.h>
#include <abclass.h>

// for AbclassNet objects
template <typename T>
Rcpp::List abclass_net_fit(
    T& object,
    const arma::uvec& y,
    const arma::vec& lambda,
    const double alpha,
    const unsigned int nlambda,
    const double lambda_min_ratio,
    const unsigned int nfolds,
    const bool stratified_cv,
    const unsigned int alignment,
    const unsigned int maxit,
    const double epsilon,
    const bool varying_active_set,
    const unsigned int verbose
    )
{
    object.fit(lambda, alpha, nlambda, lambda_min_ratio,
               maxit, epsilon, varying_active_set, verbose);
    Rcpp::NumericVector lambda_vec { abclass::arma2rvec(object.lambda_) };
    if (nfolds > 0) {
        arma::uvec strata;
        if (stratified_cv) {
            strata = y;
        }
        abclass::abclass_net_cv(object, nfolds, strata, alignment);
    }
    return Rcpp::List::create(
        Rcpp::Named("coefficients") = object.coef_,
        Rcpp::Named("weight") = abclass::arma2rvec(object.obs_weight_),
        Rcpp::Named("cross_validation") = Rcpp::List::create(
            Rcpp::Named("nfolds") = nfolds,
            Rcpp::Named("stratified") = stratified_cv,
            Rcpp::Named("alignment") = alignment,
            Rcpp::Named("cv_accuracy") = object.cv_accuracy_,
            Rcpp::Named("cv_accuracy_mean") =
            abclass::arma2rvec(object.cv_accuracy_mean_),
            Rcpp::Named("cv_accuracy_sd") =
            abclass::arma2rvec(object.cv_accuracy_sd_)
            ),
        Rcpp::Named("regularization") = Rcpp::List::create(
            Rcpp::Named("lambda") = abclass::arma2rvec(lambda_vec),
            Rcpp::Named("alpha") = alpha,
            Rcpp::Named("l1_lambda_max") = object.l1_lambda_max_
            )
        );
}

// for AbclassGroupLasso objects
template <typename T>
Rcpp::List abclass_group_lasso_fit(
    T& object,
    const arma::uvec& y,
    const arma::vec& lambda,
    const unsigned int nlambda,
    const double lambda_min_ratio,
    const arma::vec& group_weight,
    const unsigned int nfolds,
    const bool stratified_cv,
    const unsigned int alignment,
    const unsigned int maxit,
    const double epsilon,
    const bool varying_active_set,
    const unsigned int verbose
    )
{
    object.fit(lambda, nlambda, lambda_min_ratio, group_weight,
               maxit, epsilon, varying_active_set, verbose);
    Rcpp::NumericVector lambda_vec { abclass::arma2rvec(object.lambda_) };
    if (nfolds > 0) {
        arma::uvec strata;
        if (stratified_cv) {
            strata = y;
        }
        abclass::abclass_group_lasso_cv(object, nfolds, strata, alignment);
    }
    return Rcpp::List::create(
        Rcpp::Named("coefficients") = object.coef_,
        Rcpp::Named("weight") = abclass::arma2rvec(object.obs_weight_),
        Rcpp::Named("cross_validation") = Rcpp::List::create(
            Rcpp::Named("nfolds") = nfolds,
            Rcpp::Named("stratified") = stratified_cv,
            Rcpp::Named("alignment") = alignment,
            Rcpp::Named("cv_accuracy") = object.cv_accuracy_,
            Rcpp::Named("cv_accuracy_mean") =
            abclass::arma2rvec(object.cv_accuracy_mean_),
            Rcpp::Named("cv_accuracy_sd") =
            abclass::arma2rvec(object.cv_accuracy_sd_)
            ),
        Rcpp::Named("regularization") = Rcpp::List::create(
            Rcpp::Named("lambda") = abclass::arma2rvec(lambda_vec),
            Rcpp::Named("group_weight") =
            abclass::arma2rvec(object.group_weight_),
            Rcpp::Named("lambda_max") = object.lambda_max_
            )
        );
}

// for AbclassGroupSCAD/AbclassGroupMCP objects
template <typename T>
Rcpp::List abclass_group_ncv_fit(
    T& object,
    const arma::uvec& y,
    const arma::vec& lambda,
    const unsigned int nlambda,
    const double lambda_min_ratio,
    const arma::vec& group_weight,
    const double dgamma,
    const unsigned int nfolds,
    const bool stratified_cv,
    const unsigned int alignment,
    const unsigned int maxit,
    const double epsilon,
    const bool varying_active_set,
    const unsigned int verbose
    )
{
    object.fit(lambda, nlambda, lambda_min_ratio, group_weight, dgamma,
               maxit, epsilon, varying_active_set, verbose);
    Rcpp::NumericVector lambda_vec { abclass::arma2rvec(object.lambda_) };
    if (nfolds > 0) {
        arma::uvec strata;
        if (stratified_cv) {
            strata = y;
        }
        abclass::abclass_group_ncv_cv(object, nfolds, strata, alignment);
    }
    return Rcpp::List::create(
        Rcpp::Named("coefficients") = object.coef_,
        Rcpp::Named("weight") = abclass::arma2rvec(object.obs_weight_),
        Rcpp::Named("cross_validation") = Rcpp::List::create(
            Rcpp::Named("nfolds") = nfolds,
            Rcpp::Named("stratified") = stratified_cv,
            Rcpp::Named("alignment") = alignment,
            Rcpp::Named("cv_accuracy") = object.cv_accuracy_,
            Rcpp::Named("cv_accuracy_mean") =
            abclass::arma2rvec(object.cv_accuracy_mean_),
            Rcpp::Named("cv_accuracy_sd") =
            abclass::arma2rvec(object.cv_accuracy_sd_)
            ),
        Rcpp::Named("regularization") = Rcpp::List::create(
            Rcpp::Named("lambda") = abclass::arma2rvec(lambda_vec),
            Rcpp::Named("group_weight") =
            abclass::arma2rvec(object.group_weight_),
            Rcpp::Named("dgamma") = dgamma,
            Rcpp::Named("gamma") = object.gamma_,
            Rcpp::Named("lambda_max") = object.lambda_max_
            )
        );
}
