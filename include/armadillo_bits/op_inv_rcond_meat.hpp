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


//! \addtogroup op_inv_rcond
//! @{



template<typename T1>
inline
bool
op_inv_rcond::apply_direct_gen(Mat<typename T1::elem_type>& out_inv, typename T1::pod_type& out_rcond, const Base<typename T1::elem_type,T1>& expr)
  {
  arma_extra_debug_sigprint();
  
  // NOTE: this is a temporary and rudimentary implementation
  
  typedef typename T1::elem_type eT;
  typedef typename T1::pod_type   T;
  
  const Mat<eT> A = expr.get_ref();
  
  arma_debug_check( (A.is_square() == false), "inv_sympd(): given matrix must be square sized" );
  
  const bool status = op_inv_gen::apply_direct<T1,false>(out_inv, A, uword(0));
  
  if(status)
    {
    out_rcond = op_cond::rcond(expr.get_ref());
    }
  else
    {
    out_rcond = T(0);
    }
  
  return status;
  }



template<typename T1>
inline
bool
op_inv_rcond::apply_direct_spd(Mat<typename T1::elem_type>& out_inv, typename T1::pod_type& out_rcond, const Base<typename T1::elem_type,T1>& expr)
  {
  arma_extra_debug_sigprint();
  
  // NOTE: this is a temporary and rudimentary implementation
  
  typedef typename T1::elem_type eT;
  typedef typename T1::pod_type   T;
  
  const Mat<eT> A = expr.get_ref();
  
  arma_debug_check( (A.is_square() == false), "inv_sympd(): given matrix must be square sized" );
  
  const bool status = op_inv_spd::apply_direct<T1,false>(out_inv, A, uword(0));
  
  if(status)
    {
    out_rcond = op_cond::rcond(expr.get_ref());
    }
  else
    {
    out_rcond = T(0);
    }
  
  return status;
  }



//! @}
