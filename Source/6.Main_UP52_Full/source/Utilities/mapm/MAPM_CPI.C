
#include "m_apm_lc.h"

void	M_check_PI_places(int places)
{
int     dplaces;

dplaces = places + 2;

if (dplaces > MM_lc_PI_digits)
  {
   MM_lc_PI_digits = dplaces + 2;

   M_calculate_PI_AGM(MM_lc_PI, (dplaces + 5));

   m_apm_multiply(MM_lc_HALF_PI, MM_0_5, MM_lc_PI);
   m_apm_multiply(MM_lc_2_PI, MM_Two, MM_lc_PI);
  }
}

void	M_calculate_PI_AGM(M_APM outv, int places)
{
M_APM   tmp1, tmp2, a0, b0, c0, a1, b1, sum, pow_2;
int     dplaces, nn;

tmp1  = M_get_stack_var();
tmp2  = M_get_stack_var();
a0    = M_get_stack_var();
b0    = M_get_stack_var();
c0    = M_get_stack_var();
a1    = M_get_stack_var();
b1    = M_get_stack_var();
sum   = M_get_stack_var();
pow_2 = M_get_stack_var();

dplaces = places + 16;

m_apm_copy(a0, MM_One);
m_apm_copy(sum, MM_One);
m_apm_copy(pow_2, MM_Four);
m_apm_sqrt(b0, dplaces, MM_0_5);

while (TRUE)
  {
   m_apm_add(tmp1, a0, b0);
   m_apm_multiply(a1, MM_0_5, tmp1);

   m_apm_multiply(tmp1, a0, b0);
   m_apm_sqrt(b1, dplaces, tmp1);

   m_apm_subtract(tmp1, a0, b0);
   m_apm_multiply(c0, MM_0_5, tmp1);

   nn = -4 * c0->m_apm_exponent;

   m_apm_multiply(tmp1, c0, c0);
   m_apm_multiply(tmp2, tmp1, pow_2);
   m_apm_subtract(tmp1, sum, tmp2);
   m_apm_round(sum, dplaces, tmp1);

   if (nn >= dplaces)
     break;

   m_apm_copy(a0, a1);
   m_apm_copy(b0, b1);

   m_apm_multiply(tmp1, pow_2, MM_Two);
   m_apm_copy(pow_2, tmp1);
  }

m_apm_add(tmp1, a1, b1);
m_apm_multiply(tmp2, tmp1, tmp1);
m_apm_divide(tmp1, dplaces, tmp2, sum);
m_apm_round(outv, places, tmp1);

M_restore_stack(9);
}
