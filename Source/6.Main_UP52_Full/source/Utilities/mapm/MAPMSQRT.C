
#include "m_apm_lc.h"

void	m_apm_sqrt(M_APM rr, int places, M_APM aa)
{
M_APM   last_x, guess, tmpN, tmp7, tmp8, tmp9;
int	ii, bflag, nexp, tolerance, dplaces;

if (aa->m_apm_sign <= 0)
  {
   if (aa->m_apm_sign == -1)
     {
      M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_sqrt\', Negative argument");
     }

   M_set_to_zero(rr);
   return;
  }

last_x = M_get_stack_var();
guess  = M_get_stack_var();
tmpN   = M_get_stack_var();
tmp7   = M_get_stack_var();
tmp8   = M_get_stack_var();
tmp9   = M_get_stack_var();

m_apm_copy(tmpN, aa);

nexp = aa->m_apm_exponent / 2;
tmpN->m_apm_exponent -= 2 * nexp;

M_get_sqrt_guess(guess, tmpN);

tolerance = places + 4;
dplaces   = places + 16;
bflag     = FALSE;

m_apm_negate(last_x, MM_Ten);

ii = 0;

while (TRUE)
  {
   m_apm_multiply(tmp9, tmpN, guess);
   m_apm_multiply(tmp8, tmp9, guess);
   m_apm_round(tmp7, dplaces, tmp8);
   m_apm_subtract(tmp9, MM_Three, tmp7);
   m_apm_multiply(tmp8, tmp9, guess);
   m_apm_multiply(tmp9, tmp8, MM_0_5);

   if (bflag)
     break;

   m_apm_round(guess, dplaces, tmp9);

   if (ii != 0)
     {
      m_apm_subtract(tmp7, guess, last_x);

      if (tmp7->m_apm_sign == 0)
        break;

      if ((-4 * tmp7->m_apm_exponent) > tolerance)
        bflag = TRUE;
     }

   m_apm_copy(last_x, guess);
   ii++;
  }

m_apm_multiply(tmp8, tmp9, tmpN);
m_apm_round(rr, places, tmp8);
rr->m_apm_exponent += nexp;

M_restore_stack(6);
}
