
#include "m_apm_lc.h"

void	m_apm_arcsinh(M_APM rr, int places, M_APM aa)
{
M_APM	tmp0, tmp1, tmp2;

if (aa->m_apm_sign == 0)
  {
   M_set_to_zero(rr);
   return;
  }

tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();

m_apm_absolute_value(tmp0, aa);
m_apm_multiply(tmp1, tmp0, tmp0);
m_apm_add(tmp2, tmp1, MM_One);
m_apm_sqrt(tmp1, (places + 6), tmp2);
m_apm_add(tmp2, tmp0, tmp1);
m_apm_log(rr, places, tmp2);

rr->m_apm_sign = aa->m_apm_sign;

M_restore_stack(3);
}

void	m_apm_arccosh(M_APM rr, int places, M_APM aa)
{
M_APM	tmp1, tmp2;
int     ii;

ii = m_apm_compare(aa, MM_One);

if (ii == -1)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_arccosh\', Argument < 1");
   M_set_to_zero(rr);
   return;
  }

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();

m_apm_multiply(tmp1, aa, aa);
m_apm_subtract(tmp2, tmp1, MM_One);
m_apm_sqrt(tmp1, (places + 6), tmp2);
m_apm_add(tmp2, aa, tmp1);
m_apm_log(rr, places, tmp2);

M_restore_stack(2);
}

void	m_apm_arctanh(M_APM rr, int places, M_APM aa)
{
M_APM	tmp1, tmp2, tmp3;
int     ii, local_precision;

tmp1 = M_get_stack_var();

m_apm_absolute_value(tmp1, aa);

ii = m_apm_compare(tmp1, MM_One);

if (ii >= 0)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_arctanh\', |Argument| >= 1");
   M_set_to_zero(rr);
   M_restore_stack(1);
   return;
  }

tmp2 = M_get_stack_var();
tmp3 = M_get_stack_var();

local_precision = places + 8;

m_apm_add(tmp1, MM_One, aa);
m_apm_subtract(tmp2, MM_One, aa);
m_apm_divide(tmp3, local_precision, tmp1, tmp2);
m_apm_log(tmp2, local_precision, tmp3);
m_apm_multiply(tmp1, tmp2, MM_0_5);
m_apm_round(rr, places, tmp1);

M_restore_stack(3);
}
