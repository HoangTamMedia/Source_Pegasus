
#include "m_apm_lc.h"

void	M_5x_sin(M_APM r, int places, M_APM x)
{
M_APM   tmp8, tmp9;

tmp8 = M_get_stack_var();
tmp9 = M_get_stack_var();

m_apm_multiply(tmp9, x, MM_5x_125R);
M_raw_sin(tmp8, (places + 6), tmp9);
M_5x_do_it(tmp9, (places + 4), tmp8);
M_5x_do_it(tmp8, (places + 4), tmp9);
M_5x_do_it(r, places, tmp8);

M_restore_stack(2);
}

void	M_4x_cos(M_APM r, int places, M_APM x)
{
M_APM   tmp8, tmp9;

tmp8 = M_get_stack_var();
tmp9 = M_get_stack_var();

if (x->m_apm_exponent > 0)
  {
   m_apm_multiply(tmp9, x, MM_5x_256R);
   M_raw_cos(tmp8, (places + 8), tmp9);
   M_4x_do_it(tmp9, (places + 8), tmp8);
   M_4x_do_it(tmp8, (places + 6), tmp9);
   M_4x_do_it(tmp9, (places + 4), tmp8);
   M_4x_do_it(r, places, tmp9);
  }
else
  {
   m_apm_multiply(tmp9, x, MM_5x_64R);
   M_raw_cos(tmp8, (places + 6), tmp9);
   M_4x_do_it(tmp9, (places + 4), tmp8);
   M_4x_do_it(tmp8, (places + 4), tmp9);
   M_4x_do_it(r, places, tmp8);
  }

M_restore_stack(2);
}

void	M_5x_do_it(M_APM rr, int places, M_APM xx)
{
M_APM   tmp0, tmp1, t2, t3, t5;

tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();
t2   = M_get_stack_var();
t3   = M_get_stack_var();
t5   = M_get_stack_var();

m_apm_multiply(tmp1, xx, xx);
m_apm_round(t2, (places + 4), tmp1);

m_apm_multiply(tmp1, t2, xx);
m_apm_round(t3, (places + 4), tmp1);

m_apm_multiply(t5, t2, t3);

m_apm_multiply(tmp0, xx, MM_Five);
m_apm_multiply(tmp1, t5, MM_5x_Sixteen);
m_apm_add(t2, tmp0, tmp1);
m_apm_multiply(tmp1, t3, MM_5x_Twenty);
m_apm_subtract(tmp0, t2, tmp1);

m_apm_round(rr, places, tmp0);
M_restore_stack(5);
}

void	M_4x_do_it(M_APM rr, int places, M_APM xx)
{
M_APM   tmp0, tmp1, t2, t4;

tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();
t2   = M_get_stack_var();
t4   = M_get_stack_var();

m_apm_multiply(tmp1, xx, xx);
m_apm_round(t2, (places + 4), tmp1);
m_apm_multiply(t4, t2, t2);

m_apm_subtract(tmp0, t4, t2);
m_apm_multiply(tmp1, tmp0, MM_5x_Eight);
m_apm_add(tmp0, MM_One, tmp1);
m_apm_round(rr, places, tmp0);
M_restore_stack(4);
}

void	M_cos_to_sin(M_APM r, int places, M_APM a)
{
M_APM	tmp1, tmp2;

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();

m_apm_multiply(tmp1, a, a);
m_apm_subtract(tmp2, MM_One, tmp1);
m_apm_sqrt(r, places, tmp2);
M_restore_stack(2);
}
