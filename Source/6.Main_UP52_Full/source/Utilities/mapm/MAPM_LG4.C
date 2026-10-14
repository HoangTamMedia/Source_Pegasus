
#include "m_apm_lc.h"

void	M_log_near_1(M_APM rr, int places, M_APM xx)
{
M_APM   tmp0, tmp1, tmp2, tmpS, term;
int	tolerance, dplaces, local_precision;
long    m1;

tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();
tmpS = M_get_stack_var();
term = M_get_stack_var();

tolerance = xx->m_apm_exponent - (places + 6);
dplaces   = (places + 12) - xx->m_apm_exponent;

m_apm_add(tmp0, xx, MM_Two);
m_apm_divide(tmpS, (dplaces + 6), xx, tmp0);

m_apm_copy(term, tmpS);
m_apm_multiply(tmp0, tmpS, tmpS);
m_apm_round(tmp2, (dplaces + 6), tmp0);

m1 = 3L;

while (TRUE)
  {
   m_apm_multiply(tmp0, term, tmp2);

   if ((tmp0->m_apm_exponent < tolerance) || (tmp0->m_apm_sign == 0))
     break;

   local_precision = dplaces + tmp0->m_apm_exponent;

   if (local_precision < 20)
     local_precision = 20;

   m_apm_set_long(tmp1, m1);
   m_apm_round(term, local_precision, tmp0);
   m_apm_divide(tmp0, local_precision, term, tmp1);
   m_apm_add(tmp1, tmpS, tmp0);
   m_apm_copy(tmpS, tmp1);
   m1 += 2;
  }

m_apm_multiply(tmp0, MM_Two, tmpS);
m_apm_round(rr, places, tmp0);

M_restore_stack(5);
}
