
#include "m_apm_lc.h"

void	m_apm_floor(M_APM bb, M_APM aa)
{
M_APM	mtmp;

m_apm_copy(bb, aa);

if (m_apm_is_integer(bb))
  return;

if (bb->m_apm_exponent <= 0)
  {
   if (bb->m_apm_sign < 0)
     m_apm_negate(bb, MM_One);
   else
     M_set_to_zero(bb);

   return;
  }

if (bb->m_apm_sign < 0)
  {
   mtmp = M_get_stack_var();
   m_apm_negate(mtmp, bb);

   mtmp->m_apm_datalength = mtmp->m_apm_exponent;
   M_apm_normalize(mtmp);

   m_apm_add(bb, mtmp, MM_One);
   bb->m_apm_sign = -1;
   M_restore_stack(1);
  }
else
  {
   bb->m_apm_datalength = bb->m_apm_exponent;
   M_apm_normalize(bb);
  }
}

void	m_apm_ceil(M_APM bb, M_APM aa)
{
M_APM	mtmp;

m_apm_copy(bb, aa);

if (m_apm_is_integer(bb))
  return;

if (bb->m_apm_exponent <= 0)
  {
   if (bb->m_apm_sign < 0)
     M_set_to_zero(bb);
   else
     m_apm_copy(bb, MM_One);

   return;
  }

if (bb->m_apm_sign < 0)
  {
   bb->m_apm_datalength = bb->m_apm_exponent;
   M_apm_normalize(bb);
  }
else
  {
   mtmp = M_get_stack_var();
   m_apm_copy(mtmp, bb);

   mtmp->m_apm_datalength = mtmp->m_apm_exponent;
   M_apm_normalize(mtmp);

   m_apm_add(bb, mtmp, MM_One);
   M_restore_stack(1);
  }
}
