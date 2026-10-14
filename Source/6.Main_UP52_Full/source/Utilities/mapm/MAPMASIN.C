
#include "m_apm_lc.h"

void	m_apm_arctan2(M_APM rr, int places, M_APM yy, M_APM xx)
{
M_APM   tmp5, tmp6, tmp7;
int	ix, iy;

iy = yy->m_apm_sign;
ix = xx->m_apm_sign;

if (ix == 0)
  {
   if (iy == 0)
     {
      M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_arctan2\', Both Inputs = 0");
      M_set_to_zero(rr);
      return;
     }

   M_check_PI_places(places);
   m_apm_round(rr, places, MM_lc_HALF_PI);
   rr->m_apm_sign = iy;
   return;
  }

if (iy == 0)
  {
   if (ix == 1)
     {
      M_set_to_zero(rr);
     }
   else
     {
      M_check_PI_places(places);
      m_apm_round(rr, places, MM_lc_PI);
     }

   return;
  }

tmp5 = M_get_stack_var();
tmp6 = M_get_stack_var();
tmp7 = M_get_stack_var();

m_apm_divide(tmp6, (places + 6), yy, xx);
m_apm_arctan(tmp5, (places + 6), tmp6);

if (ix == 1)
  {
   m_apm_round(rr, places, tmp5);
  }
else
  {
   M_check_PI_places(places);

   if (iy == 1)
     {
      m_apm_add(tmp7, tmp5, MM_lc_PI);
      m_apm_round(rr, places, tmp7);
     }
   else
     {
      m_apm_subtract(tmp7, tmp5, MM_lc_PI);
      m_apm_round(rr, places, tmp7);
     }
  }

M_restore_stack(3);
}

void	m_apm_arctan(M_APM rr, int places, M_APM xx)
{
M_APM   tmp8, tmp9;

if (xx->m_apm_sign == 0)
  {
   M_set_to_zero(rr);
   return;
  }

if (xx->m_apm_exponent <= -4)
  {
   M_arctan_near_0(rr, places, xx);
   return;
  }

if (xx->m_apm_exponent >= 4)
  {
   M_arctan_large_input(rr, places, xx);
   return;
  }

tmp8 = M_get_stack_var();
tmp9 = M_get_stack_var();

m_apm_multiply(tmp9, xx, xx);
m_apm_add(tmp8, tmp9, MM_One);
m_apm_sqrt(tmp9, (places + 6), tmp8);
m_apm_divide(tmp8, (places + 6), xx, tmp9);
m_apm_arcsin(rr, places, tmp8);
M_restore_stack(2);
}

void	M_arctan_large_input(M_APM rr, int places, M_APM xx)
{
M_APM	tmp1, tmp2;

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();

M_check_PI_places(places);

m_apm_divide(tmp1, (places + 6), MM_One, xx);
tmp1->m_apm_sign = 1;
m_apm_arctan(tmp2, (places + 6), tmp1);
m_apm_subtract(tmp1, MM_lc_HALF_PI, tmp2);
m_apm_round(rr, places, tmp1);

rr->m_apm_sign = xx->m_apm_sign;

M_restore_stack(2);
}

void	m_apm_arcsin(M_APM r, int places, M_APM x)
{
M_APM   tmp0, tmp1, tmp2, tmp3, current_x;
int	ii, maxiter, maxp, tolerance, local_precision;

current_x = M_get_stack_var();
tmp0      = M_get_stack_var();
tmp1      = M_get_stack_var();
tmp2      = M_get_stack_var();
tmp3      = M_get_stack_var();

m_apm_absolute_value(tmp0, x);

ii = m_apm_compare(tmp0, MM_One);

if (ii == 1)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_arcsin\', |Argument| > 1");
   M_set_to_zero(r);
   M_restore_stack(5);
   return;
  }

if (ii == 0)
  {
   M_check_PI_places(places);
   m_apm_round(r, places, MM_lc_HALF_PI);
   r->m_apm_sign = x->m_apm_sign;

   M_restore_stack(5);
   return;
  }

if (m_apm_compare(tmp0, MM_0_85) == 1)
  {
   M_cos_to_sin(tmp2, (places + 4), x);
   m_apm_arccos(r, places, tmp2);
   r->m_apm_sign = x->m_apm_sign;

   M_restore_stack(5);
   return;
  }

if (x->m_apm_sign == 0)
  {
   M_set_to_zero(r);
   M_restore_stack(5);
   return;
  }

if (x->m_apm_exponent <= -4)
  {
   M_arcsin_near_0(r, places, x);
   M_restore_stack(5);
   return;
  }

tolerance       = -(places + 4);
maxp            = places + 8 - x->m_apm_exponent;
local_precision = 20 - x->m_apm_exponent;

maxiter = (int)(log((double)(places + 2)) * 1.442695) + 3;

if (maxiter < 5)
  maxiter = 5;

M_get_asin_guess(current_x, x);

ii = 0;

while (TRUE)
  {
   M_4x_cos(tmp1, local_precision, current_x);

   M_cos_to_sin(tmp2, local_precision, tmp1);
   if (tmp2->m_apm_sign != 0)
     tmp2->m_apm_sign = current_x->m_apm_sign;

   m_apm_subtract(tmp3, tmp2, x);
   m_apm_divide(tmp0, local_precision, tmp3, tmp1);

   m_apm_subtract(tmp2, current_x, tmp0);
   m_apm_copy(current_x, tmp2);

   if (ii != 0)
     {
      if (((2 * tmp0->m_apm_exponent) < tolerance) || (tmp0->m_apm_sign == 0))
        break;
     }

   if (++ii == maxiter)
     {
      M_apm_log_error_msg(M_APM_RETURN,
            "\'m_apm_arcsin\', max iteration count reached");
      break;
     }

   local_precision *= 2;

   if (local_precision > maxp)
     local_precision = maxp;
  }

m_apm_round(r, places, current_x);
M_restore_stack(5);
}

void	m_apm_arccos(M_APM r, int places, M_APM x)
{
M_APM   tmp0, tmp1, tmp2, tmp3, current_x;
int	ii, maxiter, maxp, tolerance, local_precision;

current_x = M_get_stack_var();
tmp0      = M_get_stack_var();
tmp1      = M_get_stack_var();
tmp2      = M_get_stack_var();
tmp3      = M_get_stack_var();

m_apm_absolute_value(tmp0, x);

ii = m_apm_compare(tmp0, MM_One);

if (ii == 1)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_arccos\', |Argument| > 1");
   M_set_to_zero(r);
   M_restore_stack(5);
   return;
  }

if (ii == 0)
  {
   if (x->m_apm_sign == 1)
     {
      M_set_to_zero(r);
     }
   else
     {
      M_check_PI_places(places);
      m_apm_round(r, places, MM_lc_PI);
     }

   M_restore_stack(5);
   return;
  }

if (m_apm_compare(tmp0, MM_0_85) == 1)
  {
   M_cos_to_sin(tmp2, (places + 4), x);

   if (x->m_apm_sign == 1)
     {
      m_apm_arcsin(r, places, tmp2);
     }
   else
     {
      M_check_PI_places(places);
      m_apm_arcsin(tmp3, (places + 4), tmp2);
      m_apm_subtract(tmp1, MM_lc_PI, tmp3);
      m_apm_round(r, places, tmp1);
     }

   M_restore_stack(5);
   return;
  }

if (x->m_apm_sign == 0)
  {
   M_check_PI_places(places);
   m_apm_round(r, places, MM_lc_HALF_PI);
   M_restore_stack(5);
   return;
  }

if (x->m_apm_exponent <= -4)
  {
   M_arccos_near_0(r, places, x);
   M_restore_stack(5);
   return;
  }

tolerance       = -(places + 4);
maxp            = places + 8;
local_precision = 18;

maxiter = (int)(log((double)(places + 2)) * 1.442695) + 3;

if (maxiter < 5)
  maxiter = 5;

M_get_acos_guess(current_x, x);

ii = 0;

while (TRUE)
  {
   M_4x_cos(tmp1, local_precision, current_x);

   M_cos_to_sin(tmp2, local_precision, tmp1);
   if (tmp2->m_apm_sign != 0)
     tmp2->m_apm_sign = current_x->m_apm_sign;

   m_apm_subtract(tmp3, tmp1, x);
   m_apm_divide(tmp0, local_precision, tmp3, tmp2);

   m_apm_add(tmp2, current_x, tmp0);
   m_apm_copy(current_x, tmp2);

   if (ii != 0)
     {
      if (((2 * tmp0->m_apm_exponent) < tolerance) || (tmp0->m_apm_sign == 0))
        break;
     }

   if (++ii == maxiter)
     {
      M_apm_log_error_msg(M_APM_RETURN,
            "\'m_apm_arccos\', max iteration count reached");
      break;
     }

   local_precision *= 2;

   if (local_precision > maxp)
     local_precision = maxp;
  }

m_apm_round(r, places, current_x);
M_restore_stack(5);
}
