
#include "m_apm_lc.h"

void	M_check_log_places(int places)
{
M_APM   tmp6, tmp7, tmp8, tmp9;
int     dplaces;

dplaces = places + 4;

if (dplaces > MM_lc_log_digits)
  {
   MM_lc_log_digits = dplaces + 4;

   tmp6 = M_get_stack_var();
   tmp7 = M_get_stack_var();
   tmp8 = M_get_stack_var();
   tmp9 = M_get_stack_var();

   dplaces += 6 + (int)log10((double)places);

   m_apm_copy(tmp7, MM_One);
   tmp7->m_apm_exponent = -places;

   M_log_AGM_R_func(tmp8, dplaces, MM_One, tmp7);

   m_apm_multiply(tmp6, tmp7, MM_0_5);

   M_log_AGM_R_func(tmp9, dplaces, MM_One, tmp6);

   m_apm_subtract(MM_lc_log2, tmp9, tmp8);

   tmp7->m_apm_exponent -= 1;

   M_log_AGM_R_func(tmp9, dplaces, MM_One, tmp7);

   m_apm_subtract(MM_lc_log10, tmp9, tmp8);
   m_apm_reciprocal(MM_lc_log10R, dplaces, MM_lc_log10);

   M_restore_stack(4);
  }
}

void	M_log_AGM_R_func(M_APM rr, int places, M_APM aa, M_APM bb)
{
M_APM   tmp1, tmp2, tmp3, tmp4, tmpC2, sum, pow_2, tmpA0, tmpB0;
int	tolerance, dplaces;

tmpA0 = M_get_stack_var();
tmpB0 = M_get_stack_var();
tmpC2 = M_get_stack_var();
tmp1  = M_get_stack_var();
tmp2  = M_get_stack_var();
tmp3  = M_get_stack_var();
tmp4  = M_get_stack_var();
sum   = M_get_stack_var();
pow_2 = M_get_stack_var();

tolerance = places + 8;
dplaces   = places + 16;

m_apm_copy(tmpA0, aa);
m_apm_copy(tmpB0, bb);
m_apm_copy(pow_2, MM_0_5);

m_apm_multiply(tmp1, aa, aa);
m_apm_multiply(tmp2, bb, bb);
m_apm_subtract(tmp3, tmp1, tmp2);
m_apm_multiply(sum, MM_0_5, tmp3);

while (TRUE)
  {
   m_apm_subtract(tmp1, tmpA0, tmpB0);
   m_apm_multiply(tmp4, MM_0_5, tmp1);
   m_apm_multiply(tmpC2, tmp4, tmp4);

   m_apm_add(tmp1, tmpA0, tmpB0);
   m_apm_multiply(tmp3, MM_0_5, tmp1);

   m_apm_multiply(tmp2, tmpA0, tmpB0);
   m_apm_sqrt(tmpB0, dplaces, tmp2);

   m_apm_round(tmpA0, dplaces, tmp3);

   m_apm_multiply(tmp2, MM_Two, pow_2);
   m_apm_copy(pow_2, tmp2);

   m_apm_multiply(tmp1, tmpC2, pow_2);
   m_apm_add(tmp3, sum, tmp1);

   if ((tmp1->m_apm_sign == 0) ||
      ((-2 * tmp1->m_apm_exponent) > tolerance))
     break;

   m_apm_round(sum, dplaces, tmp3);
  }

m_apm_subtract(tmp4, MM_One, tmp3);
m_apm_reciprocal(rr, places, tmp4);

M_restore_stack(9);
}
