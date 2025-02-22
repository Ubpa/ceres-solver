// Ceres Solver - A fast non-linear least squares minimizer
// Copyright 2019 Google Inc. All rights reserved.
// http://ceres-solver.org/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of Google Inc. nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Author: sameeragarwal@google.com (Sameer Agarwal)

#ifndef CERES_PUBLIC_FIRST_ORDER_FUNCTION_H_
#define CERES_PUBLIC_FIRST_ORDER_FUNCTION_H_

#include "ceres/internal/port.h"

#include <functional>

namespace ceres {

struct NextDirectionUpdateContext {
  bool valid = false;
  double* delta_x_history_col_next = nullptr;
  double* delta_gradient_history_col_next = nullptr;
  double* delta_x_dot_delta_gradient_next = nullptr;
  double* approximate_eigenvalue_scale_ = nullptr;
};
struct RightMultiplyContext {
  bool valid = false;
  const double* delta_x_history_col_it = nullptr;
  double delta_x_dot_delta_gradient_it = 0.;
  const double* delta_gradient_history_col_it = nullptr;
};

// A FirstOrderFunction object implements the evaluation of a function
// and its gradient.
class CERES_EXPORT FirstOrderFunction {
 public:
  virtual ~FirstOrderFunction() {}

  // cost is never null. gradient may be null. The return value
  // indicates whether the evaluation was successful or not.
  virtual bool Evaluate(const double* const parameters,
                        double* cost,
                        double* gradient) const = 0;
  virtual int NumParameters() const = 0;

  virtual bool EvaluateGradientNorms(const double* x,
                                     const double* gradient,
                                     double* gradient_squared_norm,
                                     double* gradient_max_norm) const {
    return false;
  }

  virtual bool NextDirection(
      const double* previous_search_direction,
      double previous_step_size,
      const double* current_gradient,
      const double* previous_gradient,
      std::function<NextDirectionUpdateContext(double)>
          getNextDirectionUpdateContext,
      double* approximate_eigenvalue_scale_,
      double* search_direction,
      std::function<RightMultiplyContext(void)> getRightMultiplyContext,
      bool use_approximate_eigenvalue_scaling_,
      double* search_direction_dot_current_gradient) const {
    return false;
  }
};

}  // namespace ceres

#endif  // CERES_PUBLIC_FIRST_ORDER_FUNCTION_H_
