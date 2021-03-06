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

#ifndef ABCLASS_HINGE_BOOST_NET_H
#define ABCLASS_HINGE_BOOST_NET_H

#include <RcppArmadillo.h>
#include "AbclassNet.h"
#include "utils.h"

namespace abclass
{
    // define class for inputs and outputs
    class HingeBoostNet : public AbclassNet
    {
    private:
        // cache
        double lum_cp1_;
        double lum_c_cp1_;

    protected:

        double lum_c_ = 0.0;

        // set CMD lowerbound
        inline void set_cmd_lowerbound() override
        {
            arma::mat sqx { arma::square(x_) };
            sqx.each_col() %= obs_weight_;
            cmd_lowerbound_ = lum_cp1_ * arma::sum(sqx, 0) / dn_obs_;
        }

        // objective function without regularization
        inline double objective0(const arma::vec& inner) const override
        {
            arma::vec tmp { arma::zeros(inner.n_elem) };
            for (size_t i {0}; i < inner.n_elem; ++i) {
                if (inner[i] < lum_c_cp1_) {
                    tmp[i] = 1.0 - inner[i];
                } else {
                    tmp[i] = std::exp(- (lum_cp1_ * inner[i] - lum_c_)) /
                        lum_cp1_;
                }
            }
            return arma::mean(obs_weight_ % tmp);
        }

        // the first derivative of the loss function
        inline arma::vec loss_derivative(const arma::vec& u) const override
        {
            arma::vec out { - arma::ones(u.n_elem) };
            for (size_t i {0}; i < u.n_elem; ++i) {
                if (u[i] > lum_c_cp1_) {
                    out[i] = - std::exp(- (lum_cp1_ * u[i] - lum_c_));
                }
            }
            return out;
        }

    public:

        // inherit default constructors
        using AbclassNet::AbclassNet;

        //! @param x The design matrix without an intercept term.
        //! @param y The category index vector.
        HingeBoostNet(const arma::mat& x,
                      const arma::uvec& y,
                      const bool intercept = true,
                      const bool standardize = true,
                      const arma::vec& weight = arma::vec()) :
            AbclassNet(x, y, intercept, standardize, weight)
        {
            set_lum_c(0.0);
        }

        HingeBoostNet* set_lum_c(const double lum_c)
        {
            if (is_lt(lum_c, 0.0)) {
                throw std::range_error("The LUM 'C' cannot be negative.");
            }
            lum_c_ = lum_c;
            lum_cp1_ = lum_c + 1.0;
            lum_c_cp1_ = lum_c_ / lum_cp1_;
            return this;
        }

    };                          // end of class

}

#endif
