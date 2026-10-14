
#include "m_apm_lc.h"

void	m_apm_cbrt(M_APM rr, int places, M_APM aa)
{
M_APM   last_x, guess, tmpN, tmp7, tmp8, tmp9;
int	ii, nexp, bflag, tolerance, maxp, local_precision;

if (aa->m_apm_sign == 0)
  {
   M_set_to_zero(rr);
   return;
  }

last_x = M_get_stack_var();
guess  = M_get_stack_var();
tmpN   = M_get_stack_var();
tmp7   = M_get_stack_var();
tmp8   = M_get_stack_var();
tmp9   = M_get_stack_var();

m_apm_absolute_value(tmpN, aa);

nexp = aa->m_apm_exponent / 3;
tmpN->m_apm_exponent -= 3 * nexp;

M_get_cbrt_guess(guess, tmpN);

tolerance       = places + 4;
maxp            = places + 16;
bflag           = FALSE;
local_precision = 14;

m_apm_negate(last_x, MM_Ten);

ii = 0;

while (TRUE)
  {
   m_apm_multiply(tmp8, guess, guess);
   m_apm_multiply(tmp7, tmp8, tmp8);
   m_apm_round(tmp8, local_precision, tmp7);
   m_apm_multiply(tmp9, tmpN, tmp8);

   m_apm_multiply(tmp8, MM_Four, guess);
   m_apm_subtract(tmp7, tmp8, tmp9);
   m_apm_divide(guess, local_precision, tmp7, MM_Three);

   if (bflag)
     break;

   if (ii != 0)
     {
      m_apm_subtract(tmp8, guess, last_x);

      if (tmp8->m_apm_sign == 0)
        break;

      if ((-4 * tmp8->m_apm_exponent) > tolerance)
        bflag = TRUE;
     }

   local_precision *= 2;

   if (local_precision > maxp)
     local_precision = maxp;

   m_apm_copy(last_x, guess);
   ii = 1;
  }

m_apm_multiply(tmp9, guess, guess);
m_apm_multiply(tmp8, tmp9, tmpN);
m_apm_round(rr, places, tmp8);

rr->m_apm_exponent += nexp;
rr->m_apm_sign = aa->m_apm_sign;
M_restore_stack(6);
}
