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


//! \addtogroup fn_rank
//! @{



template<typename eT>
arma_warn_unused
inline
typename enable_if2< is_supported_blas_type<eT>::value, uword >::result
rank_diagmat
  (
  const Mat<eT>&                          A,
        typename get_pod_type<eT>::result tol = 0.0
  )
  {
  arma_extra_debug_sigprint();
  
  typedef typename get_pod_type<eT>::result T;
  
  if(A.is_empty())  { return uword(0); }
  
  const uword N = (std::min)(A.n_rows, A.n_cols);
  
  podarray<T> diag_abs_vals(N);
  
  T max_abs_Aii = T(0);
  
  for(uword i=0; i<N; ++i)
    {
    const eT     Aii = A.at(i,i);
    const  T abs_Aii = std::abs(Aii);
    
    if(arma_isnan(Aii))  { arma_stop_runtime_error("rank(): detected NaN"); return uword(0); }
    
    diag_abs_vals[i] = abs_Aii;
    
    max_abs_Aii = (abs_Aii > max_abs_Aii) ? abs_Aii : max_abs_Aii;
    }
  
  if(tol == T(0))  { tol = (std::max)(A.n_rows, A.n_cols) * max_abs_Aii * std::numeric_limits<T>::epsilon(); }
  
  uword count = 0;
  
  for(uword i=0; i<N; ++i)  { count += (diag_abs_vals[i] > tol) ? uword(1) : uword(0); }
  
  return count;
  }



template<typename T1>
arma_warn_unused
inline
typename enable_if2< is_supported_blas_type<typename T1::elem_type>::value, uword >::result
rank_sym
  (
  const Base<typename T1::elem_type,T1>& X,
        typename T1::pod_type            tol = 0.0
  )
  {
  arma_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  typedef typename T1::pod_type   T;
  
  Mat<eT> A(X.get_ref());
  
  if(A.is_square() == false)
    {
    arma_stop_runtime_error("rank_sym(): given matrix must be square sized");
    return uword(0);
    }
  
  if(A.is_empty())  { return uword(0); }
  
  // if(is_op_diagmat<T1>::value || A.is_diagmat())
  //   {
  //   arma_extra_debug_print("rank(): detected diagonal matrix");
  //   // TODO: return rank_diagmat(A, tol);
  //   }
  
  Col<T> v;
  
  const bool status = auxlib::eig_sym(v, A);
  
  if(status == false)
    {
    arma_stop_runtime_error("rank_sym(): eig_sym failed");
    return uword(0);
    }
  
  const uword v_n_elem = v.n_elem;
  const T*    v_mem    = v.memptr();
  
  // set tolerance to default if it hasn't been specified
  if( (tol == T(0)) && (v_n_elem > 0) )
    {
    tol = (std::max)(A.n_rows, A.n_cols) * v_mem[v_n_elem-1] * std::numeric_limits<T>::epsilon();
    }
  
  uword count = 0;
  
  for(uword i=0; i < v_n_elem; ++i)  { count += (v_mem[i] > tol) ? uword(1) : uword(0); }
  
  return count;
  }



template<typename T1>
arma_warn_unused
inline
uword
rank
  (
  const Base<typename T1::elem_type,T1>& X,
        typename T1::pod_type            tol = 0.0,
  const typename arma_blas_type_only<typename T1::elem_type>::result* junk = nullptr
  )
  {
  arma_extra_debug_sigprint();
  arma_ignore(junk);
  
  typedef typename T1::elem_type eT;
  typedef typename T1::pod_type   T;
  
  Mat<eT> A(X.get_ref());
  
  if(A.is_empty())  { return uword(0); }
  
  if(is_op_diagmat<T1>::value || A.is_diagmat())
    {
    arma_extra_debug_print("rank(): detected diagonal matrix");
    
    return rank_diagmat(A, tol);
    }
  
  if(is_cx<eT>::no && A.is_symmetric())
    {
    arma_extra_debug_print("rank(): detected symmetric matrix");
    
    return rank_sym(A, tol);
    }
  
  if(is_cx<eT>::yes && A.is_hermitian())
    {
    arma_extra_debug_print("rank(): detected hermitian matrix");
    
    return rank_sym(A, tol);
    }
  
  Col<T> s;
  
  const bool status = auxlib::svd_dc(s, A);
  
  if(status == false)
    {
    arma_stop_runtime_error("rank(): svd failed");
    
    return uword(0);
    }
  
  const uword s_n_elem = s.n_elem;
  const T*    s_mem    = s.memptr();
  
  // set tolerance to default if it hasn't been specified
  if( (tol == T(0)) && (s_n_elem > 0) )
    {
    tol = (std::max)(A.n_rows, A.n_cols) * s_mem[0] * std::numeric_limits<T>::epsilon();
    }
  
  uword count = 0;
  
  for(uword i=0; i < s_n_elem; ++i)  { count += (s_mem[i] > tol) ? uword(1) : uword(0); }
  
  return count;
  }



//! @}
