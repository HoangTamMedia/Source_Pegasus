
#include "m_apm_lc.h"

void	m_apm_integer_pow_nr(M_APM rr, M_APM aa, int mexp)
{
M_APM   tmp0, tmpy, tmpz;
int	nexp, ii;

if (mexp == 0)
  {
   m_apm_copy(rr, MM_One);
   return;
  }
else
  {
   if (mexp < 0)
     {
      M_apm_log_error_msg(M_APM_RETURN,
            "\'m_apm_integer_pow_nr\', Negative exponent");

      M_set_to_zero(rr);
      return;
     }
  }

if (mexp == 1)
  {
   m_apm_copy(rr, aa);
   return;
  }

if (mexp == 2)
  {
   m_apm_multiply(rr, aa, aa);
   return;
  }

nexp = mexp;

if (aa->m_apm_sign == 0)
  {
   M_set_to_zero(rr);
   return;
  }

tmp0 = M_get_stack_var();
tmpy = M_get_stack_var();
tmpz = M_get_stack_var();

m_apm_copy(tmpy, MM_One);
m_apm_copy(tmpz, aa);

while (TRUE)
  {
   ii   = nexp & 1;
   nexp = nexp >> 1;

   if (ii != 0)
     {
      m_apm_multiply(tmp0, tmpy, tmpz);

      if (nexp == 0)
        break;

      m_apm_copy(tmpy, tmp0);
     }

   m_apm_multiply(tmp0, tmpz, tmpz);
   m_apm_copy(tmpz, tmp0);
  }

m_apm_copy(rr, tmp0);

M_restore_stack(3);
}
