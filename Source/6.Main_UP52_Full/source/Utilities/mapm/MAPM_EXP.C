
#include "m_apm_lc.h"

static  M_APM  MM_exp_log2R;
static  M_APM  MM_exp_512R;
static	int    MM_firsttime1 = TRUE;

void	M_free_all_exp()
{
if (MM_firsttime1 == FALSE)
  {
   m_apm_free(MM_exp_log2R);
   m_apm_free(MM_exp_512R);

   MM_firsttime1 = TRUE;
  }
}

void	m_apm_exp(M_APM r, int places, M_APM x)
{
M_APM   tmp7, tmp8, tmp9;
int	dplaces, nn, ii;

if (MM_firsttime1)
  {
   MM_firsttime1 = FALSE;

   MM_exp_log2R = m_apm_init();
   MM_exp_512R  = m_apm_init();

   m_apm_set_string(MM_exp_log2R, "1.44269504089");
   m_apm_set_string(MM_exp_512R,  "1.953125E-3");
  }

tmp7 = M_get_stack_var();
tmp8 = M_get_stack_var();
tmp9 = M_get_stack_var();

if (x->m_apm_sign == 0)
  {
   m_apm_copy(r, MM_One);
   M_restore_stack(3);
   return;
  }

if (x->m_apm_exponent <= -3)
  {
   M_raw_exp(tmp9, (places + 6), x);
   m_apm_round(r, places, tmp9);
   M_restore_stack(3);
   return;
  }

if (M_exp_compute_nn(&nn, tmp7, x) != 0)
  {
   M_apm_log_error_msg(M_APM_RETURN,
      "\'m_apm_exp\', Input too large, Overflow");

   M_set_to_zero(r);
   M_restore_stack(3);
   return;
  }

dplaces = places + 8;

M_check_log_places(dplaces);

m_apm_multiply(tmp8, tmp7, MM_lc_log2);
m_apm_subtract(tmp7, x, tmp8);

while (TRUE)
  {
   if (tmp7->m_apm_sign != 0)
     {
      if (tmp7->m_apm_exponent == 0)
        break;
     }

   if (tmp7->m_apm_sign >= 0)
     {
      nn++;
      m_apm_subtract(tmp8, tmp7, MM_lc_log2);
      m_apm_copy(tmp7, tmp8);
     }
   else
     {
      nn--;
      m_apm_add(tmp8, tmp7, MM_lc_log2);
      m_apm_copy(tmp7, tmp8);
     }
  }

m_apm_multiply(tmp9, tmp7, MM_exp_512R);

M_raw_exp(tmp8, dplaces, tmp9);

ii = 9;

while (TRUE)
  {
   m_apm_multiply(tmp9, tmp8, tmp8);
   m_apm_round(tmp8, dplaces, tmp9);

   if (--ii == 0)
     break;
  }

m_apm_integer_pow(tmp7, dplaces, MM_Two, nn);

m_apm_multiply(tmp9, tmp7, tmp8);
m_apm_round(r, places, tmp9);

M_restore_stack(3);
}

int	M_exp_compute_nn(int *n, M_APM b, M_APM a)
{
M_APM	tmp0, tmp1;
void	*vp;
char    *cp, sbuf[48];
int	kk;

*n   = 0;
vp   = NULL;
cp   = sbuf;
tmp0 = M_get_stack_var();
tmp1 = M_get_stack_var();

m_apm_multiply(tmp1, a, MM_exp_log2R);

if (tmp1->m_apm_sign >= 0)
  {
   m_apm_add(tmp0, tmp1, MM_0_5);
   m_apm_floor(tmp1, tmp0);
  }
else
  {
   m_apm_subtract(tmp0, tmp1, MM_0_5);
   m_apm_ceil(tmp1, tmp0);
  }

kk = tmp1->m_apm_exponent;
if (kk >= 42)
  {
   if ((vp = (void *)MAPM_MALLOC((kk + 16) * sizeof(char))) == NULL)
     {

      M_apm_log_error_msg(M_APM_FATAL, "\'M_exp_compute_nn\', Out of memory");
     }

   cp = (char *)vp;
  }

m_apm_to_integer_string(cp, tmp1);
*n = atoi(cp);

m_apm_set_long(b, (long)(*n));

kk = m_apm_compare(b, tmp1);

if (vp != NULL)
  MAPM_FREE(vp);

M_restore_stack(2);
return(kk);
}

void	M_raw_exp(M_APM rr, int places, M_APM xx)
{
M_APM   tmp0, digit, term;
int	tolerance,  local_precision, prev_exp;
long    m1;

tmp0  = M_get_stack_var();
term  = M_get_stack_var();
digit = M_get_stack_var();

local_precision = places + 8;
tolerance       = -(places + 4);
prev_exp        = 0;

m_apm_add(rr, MM_One, xx);
m_apm_copy(term, xx);

m1 = 2L;

while (TRUE)
  {
   m_apm_set_long(digit, m1);
   m_apm_multiply(tmp0, term, xx);
   m_apm_divide(term, local_precision, tmp0, digit);
   m_apm_add(tmp0, rr, term);
   m_apm_copy(rr, tmp0);

   if ((term->m_apm_exponent < tolerance) || (term->m_apm_sign == 0))
     break;

   if (m1 != 2L)
     {
      local_precision = local_precision + term->m_apm_exponent - prev_exp;

      if (local_precision < 20)
        local_precision = 20;
     }

   prev_exp = term->m_apm_exponent;
   m1++;
  }

M_restore_stack(3);
}
