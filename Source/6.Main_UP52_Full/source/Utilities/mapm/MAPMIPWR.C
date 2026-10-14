
#include "m_apm_lc.h"

void	m_apm_integer_pow(M_APM rr, int places, M_APM aa, int mexp)
{
M_APM   tmp0, tmpy, tmpz;
int	nexp, ii, signflag, local_precision;

if (mexp == 0)
  {
   m_apm_copy(rr, MM_One);
   return;
  }
else
  {
   if (mexp > 0)
     {
      signflag = 0;
      nexp     = mexp;
     }
   else
     {
      signflag = 1;
      nexp     = -mexp;
     }
  }

if (aa->m_apm_sign == 0)
  {
   M_set_to_zero(rr);
   return;
  }

tmp0 = M_get_stack_var();
tmpy = M_get_stack_var();
tmpz = M_get_stack_var();

local_precision = places + 8;

m_apm_copy(tmpy, MM_One);
m_apm_copy(tmpz, aa);

while (TRUE)
  {
   ii   = nexp & 1;
   nexp = nexp >> 1;

   if (ii != 0)
     {
      m_apm_multiply(tmp0, tmpy, tmpz);
      m_apm_round(tmpy, local_precision, tmp0);

      if (nexp == 0)
        break;
     }

   m_apm_multiply(tmp0, tmpz, tmpz);
   m_apm_round(tmpz, local_precision, tmp0);
  }

if (signflag)
  {
   m_apm_reciprocal(rr, places, tmpy);
  }
else
  {
   m_apm_round(rr, places, tmpy);
  }

M_restore_stack(3);
}
