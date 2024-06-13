// SPDX-License-Identifier: Apache-2.0
// 
// Copyright 2008-2016 Conrad Sanderson (http://conradsanderson.id.au)
// Copyright 2008-2016 National ICT Australia (NICTA)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------


//! \addtogroup op_sp_minus
//! @{



// Subtract a sparse object from a scalar; the output will be a dense object.
class op_sp_minus_pre
  {
  public:

  template<typename T1>
  struct traits
    {
    static constexpr bool is_row  = T1::is_row;
    static constexpr bool is_col  = T1::is_col;
    static constexpr bool is_xvec = T1::is_xvec;
    static constexpr bool is_d2sp = false;
    };
  
  template<typename T1>
  inline static void apply(Mat<typename T1::elem_type>& out, const SpToDOp<T1,op_sp_minus_pre>& in);

  // force apply into sparse matrix
  template<typename T1>
  inline static void apply(SpMat<typename T1::elem_type>& out, const SpToDOp<T1,op_sp_minus_pre>& in);

  // used for the optimization of sparse % (scalar - sparse)
  template<typename eT, typename T2, typename T3>
  inline static void apply_inside_schur(SpMat<eT>& out, const T2& x, const SpToDOp<T3, op_sp_minus_pre>& y);

  // used for the optimization of sparse / (scalar - sparse)
  template<typename eT, typename T2, typename T3>
  inline static void apply_inside_div(SpMat<eT>& out, const T2& x, const SpToDOp<T3, op_sp_minus_pre>& y);
  };



// Subtract a scalar from a sparse object; the output will be a dense object.
class op_sp_minus_post
  {
  public:

  template<typename T1>
  struct traits
    {
    static constexpr bool is_row  = T1::is_row;
    static constexpr bool is_col  = T1::is_col;
    static constexpr bool is_xvec = T1::is_xvec;
    static constexpr bool is_d2sp = false;
    };
  
  template<typename T1>
  inline static void apply(Mat<typename T1::elem_type>& out, const SpToDOp<T1,op_sp_minus_post>& in);

  // force apply into sparse matrix
  template<typename T1>
  inline static void apply(SpMat<typename T1::elem_type>& out, const SpToDOp<T1,op_sp_minus_post>& in);

  // used for the optimization of sparse % (sparse - scalar)
  template<typename eT, typename T2, typename T3>
  inline static void apply_inside_schur(SpMat<eT>& out, const T2& x, const SpToDOp<T3, op_sp_minus_post>& y);

  // used for the optimization of sparse / (sparse - scalar)
  template<typename eT, typename T2, typename T3>
  inline static void apply_inside_div(SpMat<eT>& out, const T2& x, const SpToDOp<T3, op_sp_minus_post>& y);
  };



//! @}
