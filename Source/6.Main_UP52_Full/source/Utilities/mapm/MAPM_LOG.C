
#include "m_apm_lc.h"

void	m_apm_log10(M_APM rr, int places, M_APM aa)
{
int     dplaces;
M_APM   tmp8, tmp9;

tmp8 = M_get_stack_var();
tmp9 = M_get_stack_var();

dplaces = places + 4;
M_check_log_places(dplaces + 45);

m_apm_log(tmp9, dplaces, aa);
m_apm_multiply(tmp8, tmp9, MM_lc_log10R);
m_apm_round(rr, places, tmp8);
M_restore_stack(2);
}

void	m_apm_log(M_APM r, int places, M_APM a)
{
M_APM   tmp0, tmp1, tmp2;
int	mexp, dplaces;

if (a->m_apm_sign <= 0)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_log\', Negative argument");
   M_set_to_zero(r);
   return;
  }

tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();

dplaces = places + 8;

mexp = a->m_apm_exponent;

if (mexp == 0 || mexp == 1)
  {
   m_apm_subtract(tmp0, a, MM_One);

   if (tmp0->m_apm_sign == 0)
     {
      M_set_to_zero(r);
      M_restore_stack(3);
      return;
     }

   if (tmp0->m_apm_exponent <= -4)
     {
      M_log_near_1(r, places, tmp0);
      M_restore_stack(3);
      return;
     }
  }

M_check_log_places(dplaces + 25);

if (abs(mexp) <= 3)
  {
   M_log_basic_iteration(r, places, a);
  }
else
  {

   m_apm_copy(tmp2, a);

   mexp = tmp2->m_apm_exponent - 2;
   tmp2->m_apm_exponent = 2;

   M_log_basic_iteration(tmp0, dplaces, tmp2);

   m_apm_set_long(tmp1, (long)mexp);
   m_apm_multiply(tmp2, tmp1, MM_lc_log10);
   m_apm_add(tmp1, tmp2, tmp0);

   m_apm_round(r, places, tmp1);
  }

M_restore_stack(3);
}
