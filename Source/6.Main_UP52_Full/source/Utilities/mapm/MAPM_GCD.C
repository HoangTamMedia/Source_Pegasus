
#include "m_apm_lc.h"

void	m_apm_gcd(M_APM r, M_APM u, M_APM v)
{
M_APM   tmpM, tmpN, tmpT, tmpU, tmpV;
int	kk, kr, mm;
long    pow_2;

if ((m_apm_is_integer(u) + m_apm_is_integer(v)) != 2)
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_gcd\', Non-integer input");

   M_set_to_zero(r);
   return;
  }

if (u->m_apm_sign == 0)
  {
   m_apm_absolute_value(r, v);
   return;
  }

if (v->m_apm_sign == 0)
  {
   m_apm_absolute_value(r, u);
   return;
  }

tmpM = M_get_stack_var();
tmpN = M_get_stack_var();
tmpT = M_get_stack_var();
tmpU = M_get_stack_var();
tmpV = M_get_stack_var();

m_apm_absolute_value(tmpU, u);
m_apm_absolute_value(tmpV, v);

kk = 0;

while (TRUE)
  {
   mm = 1;
   if (m_apm_is_odd(tmpU))
     break;

   mm = 0;
   if (m_apm_is_odd(tmpV))
     break;

   m_apm_multiply(tmpN, MM_0_5, tmpU);
   m_apm_copy(tmpU, tmpN);

   m_apm_multiply(tmpN, MM_0_5, tmpV);
   m_apm_copy(tmpV, tmpN);

   kk++;
  }

if (mm)
  {
   m_apm_negate(tmpT, tmpV);
   goto B4;
  }

m_apm_copy(tmpT, tmpU);

B3:

m_apm_multiply(tmpN, MM_0_5, tmpT);
m_apm_copy(tmpT, tmpN);

B4:

if (m_apm_is_even(tmpT))
  goto B3;

if (tmpT->m_apm_sign == 1)
  m_apm_copy(tmpU, tmpT);
else
  m_apm_negate(tmpV, tmpT);

m_apm_subtract(tmpT, tmpU, tmpV);

if (tmpT->m_apm_sign != 0)
  goto B3;

if (kk == 0)
   m_apm_copy(r, tmpU);
else
  {
   if (kk == 1)
     m_apm_multiply(r, tmpU, MM_Two);

   if (kk == 2)
     m_apm_multiply(r, tmpU, MM_Four);

   if (kk >= 3)
     {
      mm = kk / 28;
      kr = kk % 28;
      pow_2 = 1L << kr;

      if (mm == 0)
        {
	 m_apm_set_long(tmpN, pow_2);
         m_apm_multiply(r, tmpU, tmpN);
	}
      else
        {
	 m_apm_copy(tmpN, MM_One);
         m_apm_set_long(tmpM, 0x10000000L);

	 while (TRUE)
	   {
            m_apm_multiply(tmpT, tmpN, tmpM);
            m_apm_copy(tmpN, tmpT);

	    if (--mm == 0)
	      break;
	   }

	 if (kr == 0)
	   {
            m_apm_multiply(r, tmpU, tmpN);
	   }
	 else
	   {
	    m_apm_set_long(tmpM, pow_2);
            m_apm_multiply(tmpT, tmpN, tmpM);
            m_apm_multiply(r, tmpU, tmpT);
	   }
	}
     }
  }

M_restore_stack(5);
}

void	m_apm_lcm(M_APM r, M_APM u, M_APM v)
{
M_APM   tmpN, tmpG;

tmpN = M_get_stack_var();
tmpG = M_get_stack_var();

m_apm_multiply(tmpN, u, v);
m_apm_gcd(tmpG, u, v);
m_apm_integer_divide(r, tmpN, tmpG);

M_restore_stack(2);
}

#ifdef BIG_COMMENT_BLOCK

void	m_apm_gcd_traditional(M_APM r, M_APM u, M_APM v)
{
M_APM   tmpD, tmpN, tmpU, tmpV;

tmpD = M_get_stack_var();
tmpN = M_get_stack_var();
tmpU = M_get_stack_var();
tmpV = M_get_stack_var();

m_apm_absolute_value(tmpU, u);
m_apm_absolute_value(tmpV, v);

while (TRUE)
  {
   if (tmpV->m_apm_sign == 0)
     break;

   m_apm_integer_div_rem(tmpD, tmpN, tmpU, tmpV);
   m_apm_copy(tmpU, tmpV);
   m_apm_copy(tmpV, tmpN);
  }

m_apm_copy(r, tmpU);
M_restore_stack(4);
}

#endif
