
#include "m_apm_lc.h"

void	M_arcsin_near_0(M_APM rr, int places, M_APM aa)
{
M_APM   tmp5, tmp6;

tmp5 = M_get_stack_var();
tmp6 = M_get_stack_var();

M_cos_to_sin(tmp5, (places + 8), aa);
m_apm_divide(tmp6, (places + 8), aa, tmp5);
M_arctan_near_0(rr, places, tmp6);

M_restore_stack(2);
}

void	M_arccos_near_0(M_APM rr, int places, M_APM aa)
{
M_APM   tmp1, tmp2;

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();

M_check_PI_places(places);
M_arcsin_near_0(tmp1, (places + 4), aa);
m_apm_subtract(tmp2, MM_lc_HALF_PI, tmp1);
m_apm_round(rr, places, tmp2);

M_restore_stack(2);
}

void	M_arctan_near_0(M_APM rr, int places, M_APM aa)
{
M_APM   tmp0, tmp2, tmpR, tmpS, digit, term;
int	tolerance, dplaces, local_precision;
long    m1;

tmp0  = M_get_stack_var();
tmp2  = M_get_stack_var();
tmpR  = M_get_stack_var();
tmpS  = M_get_stack_var();
term  = M_get_stack_var();
digit = M_get_stack_var();

tolerance = aa->m_apm_exponent - (places + 4);
dplaces   = (places + 8) - aa->m_apm_exponent;

m_apm_copy(term, aa);
m_apm_copy(tmpS, aa);
m_apm_multiply(tmp0, aa, aa);
m_apm_round(tmp2, (dplaces + 8), tmp0);

m1 = 1L;

while (TRUE)
  {

   m_apm_multiply(tmp0, term, tmp2);

   if ((tmp0->m_apm_exponent < tolerance) || (tmp0->m_apm_sign == 0))
     {
      m_apm_round(rr, places, tmpS);
      break;
     }

   local_precision = dplaces + tmp0->m_apm_exponent;

   if (local_precision < 20)
     local_precision = 20;

   m1 += 2;
   m_apm_set_long(digit, m1);
   m_apm_round(term, local_precision, tmp0);
   m_apm_divide(tmp0, local_precision, term, digit);
   m_apm_subtract(tmpR, tmpS, tmp0);

   m_apm_multiply(tmp0, term, tmp2);

   if ((tmp0->m_apm_exponent < tolerance) || (tmp0->m_apm_sign == 0))
     {
      m_apm_round(rr, places, tmpR);
      break;
     }

   local_precision = dplaces + tmp0->m_apm_exponent;

   if (local_precision < 20)
     local_precision = 20;

   m1 += 2;
   m_apm_set_long(digit, m1);
   m_apm_round(term, local_precision, tmp0);
   m_apm_divide(tmp0, local_precision, term, digit);
   m_apm_add(tmpS, tmpR, tmp0);
  }

M_restore_stack(6);
}
