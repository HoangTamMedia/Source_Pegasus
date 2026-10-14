
#include "m_apm_lc.h"

static	M_APM   M_last_xx_input;
static	M_APM   M_last_xx_log;
static	int     M_last_log_digits;
static	int     M_size_flag = 0;

void	M_free_all_pow()
{
if (M_size_flag != 0)
  {
   m_apm_free(M_last_xx_input);
   m_apm_free(M_last_xx_log);
   M_size_flag = 0;
  }
}

void	m_apm_pow(M_APM rr, int places, M_APM xx, M_APM yy)
{
int	iflag, pflag;
char    sbuf[64];
M_APM   tmp8, tmp9;

if (yy->m_apm_sign == 0)
  {
   m_apm_copy(rr, MM_One);
   return;
  }

if (xx->m_apm_sign == 0)
  {
   M_set_to_zero(rr);
   return;
  }

if (M_size_flag == 0)
  {
   M_size_flag       = M_get_sizeof_int();
   M_last_log_digits = 0;
   M_last_xx_input   = m_apm_init();
   M_last_xx_log     = m_apm_init();
  }

if (m_apm_is_integer(yy))
  {
   iflag = FALSE;

   if (M_size_flag == 2)
     {
      if (yy->m_apm_exponent <= 4)
        iflag = TRUE;
     }
   else
     {
      if (yy->m_apm_exponent <= 7)
        iflag = TRUE;
     }

   if (iflag)
     {
      m_apm_to_integer_string(sbuf, yy);
      m_apm_integer_pow(rr, places, xx, atoi(sbuf));
      return;
     }
  }

tmp8 = M_get_stack_var();
tmp9 = M_get_stack_var();

pflag = FALSE;

if (M_last_log_digits >= places)
  {
   if (m_apm_compare(xx, M_last_xx_input) == 0)
     pflag = TRUE;
  }

if (pflag)
  {
   m_apm_round(tmp9, (places + 8), M_last_xx_log);
  }
else
  {
   m_apm_log(tmp9, (places + 8), xx);

   M_last_log_digits = places + 2;

   m_apm_copy(M_last_xx_input, xx);
   m_apm_copy(M_last_xx_log, tmp9);
  }

m_apm_multiply(tmp8, tmp9, yy);
m_apm_exp(rr, places, tmp8);
M_restore_stack(2);
}
