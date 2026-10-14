
#include "m_apm_lc.h"

void	m_apm_divide(M_APM rr, int places, M_APM aa, M_APM bb)
{
M_APM   tmp0, tmp1;
int     sn, nexp, dplaces;

sn = aa->m_apm_sign * bb->m_apm_sign;

if (sn == 0)
  {
   if (bb->m_apm_sign == 0)
     {
      M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_divide\', Divide by 0");
     }

   M_set_to_zero(rr);
   return;
  }

if (places < 250)
  {
   M_apm_sdivide(rr, places, aa, bb);
   return;
  }

nexp = aa->m_apm_exponent - bb->m_apm_exponent;

if (nexp > 0)
  dplaces = nexp + places;
else
  dplaces = places;

tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();

m_apm_reciprocal(tmp0, (dplaces + 8), bb);
m_apm_multiply(tmp1, tmp0, aa);
m_apm_round(rr, dplaces, tmp1);

M_restore_stack(2);
}

void	m_apm_reciprocal(M_APM rr, int places, M_APM aa)
{
M_APM   last_x, guess, tmpN, tmp1, tmp2;
char    sbuf[32];
int	ii, bflag, dplaces, nexp, tolerance;

if (aa->m_apm_sign == 0)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_reciprocal\', Input = 0");

   M_set_to_zero(rr);
   return;
  }

last_x = M_get_stack_var();
guess  = M_get_stack_var();
tmpN   = M_get_stack_var();
tmp1   = M_get_stack_var();
tmp2   = M_get_stack_var();

m_apm_absolute_value(tmpN, aa);

nexp = aa->m_apm_exponent;
tmpN->m_apm_exponent -= nexp;

m_apm_to_string(sbuf, 15, tmpN);
m_apm_set_double(guess, (1.0 / atof(sbuf)));

tolerance = places + 4;
dplaces   = places + 16;
bflag     = FALSE;

m_apm_negate(last_x, MM_Ten);

ii = 0;

while (TRUE)
  {
   m_apm_multiply(tmp1, tmpN, guess);
   m_apm_subtract(tmp2, MM_Two, tmp1);
   m_apm_multiply(tmp1, tmp2, guess);

   if (bflag)
     break;

   m_apm_round(guess, dplaces, tmp1);

   if (ii != 0)
     {
      m_apm_subtract(tmp2, guess, last_x);

      if (tmp2->m_apm_sign == 0)
        break;

      if ((-4 * tmp2->m_apm_exponent) > tolerance)
        bflag = TRUE;
     }

   m_apm_copy(last_x, guess);
   ii++;
  }

m_apm_round(rr, places, tmp1);
rr->m_apm_exponent -= nexp;
rr->m_apm_sign = aa->m_apm_sign;
M_restore_stack(5);
}
