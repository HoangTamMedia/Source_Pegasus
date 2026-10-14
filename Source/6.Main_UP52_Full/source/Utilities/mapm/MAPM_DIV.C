
#include "m_apm_lc.h"

static	M_APM	M_div_worka;
static	M_APM	M_div_workb;
static	M_APM	M_div_tmp7;
static	M_APM	M_div_tmp8;
static	M_APM	M_div_tmp9;

static	int	M_div_firsttime = TRUE;

void	M_free_all_div()
{
if (M_div_firsttime == FALSE)
  {
   m_apm_free(M_div_worka);
   m_apm_free(M_div_workb);
   m_apm_free(M_div_tmp7);
   m_apm_free(M_div_tmp8);
   m_apm_free(M_div_tmp9);

   M_div_firsttime = TRUE;
  }
}

void	m_apm_integer_div_rem(M_APM qq, M_APM rr, M_APM aa, M_APM bb)
{
m_apm_integer_divide(qq, aa, bb);
m_apm_multiply(M_div_tmp7, qq, bb);
m_apm_subtract(rr, aa, M_div_tmp7);
}

void	m_apm_integer_divide(M_APM rr, M_APM aa, M_APM bb)
{

M_apm_sdivide(rr, 4, aa, bb);

if (rr->m_apm_exponent <= 0)
  {
   M_set_to_zero(rr);
  }
else
  {
   if (rr->m_apm_datalength > rr->m_apm_exponent)
     {
      rr->m_apm_datalength = rr->m_apm_exponent;
      M_apm_normalize(rr);
     }
  }
}

void	M_apm_sdivide(M_APM r, int places, M_APM a, M_APM b)
{
int	j, k, m, b0, sign, nexp, indexr, icompare, iterations;
long    trial_numer;
void	*vp;

if (M_div_firsttime)
  {
   M_div_firsttime = FALSE;

   M_div_worka = m_apm_init();
   M_div_workb = m_apm_init();
   M_div_tmp7  = m_apm_init();
   M_div_tmp8  = m_apm_init();
   M_div_tmp9  = m_apm_init();
  }

sign = a->m_apm_sign * b->m_apm_sign;

if (sign == 0)
  {
   if (b->m_apm_sign == 0)
     {
      M_apm_log_error_msg(M_APM_RETURN, "\'M_apm_sdivide\', Divide by 0");
     }

   M_set_to_zero(r);
   return;
  }

if (b->m_apm_data[0] >= 50)
  {
   m_apm_absolute_value(M_div_worka, a);
   m_apm_absolute_value(M_div_workb, b);
  }
else
  {
   k = 100 / (b->m_apm_data[0] + 1);
   m_apm_set_long(M_div_tmp9, (long)k);

   m_apm_multiply(M_div_worka, M_div_tmp9, a);
   m_apm_multiply(M_div_workb, M_div_tmp9, b);

   M_div_worka->m_apm_sign = 1;
   M_div_workb->m_apm_sign = 1;
  }

b0 = 100 * (int)M_div_workb->m_apm_data[0];

if (M_div_workb->m_apm_datalength >= 3)
  b0 += M_div_workb->m_apm_data[1];

nexp = M_div_worka->m_apm_exponent - M_div_workb->m_apm_exponent;

if (nexp > 0)
  iterations = nexp + places + 1;
else
  iterations = places + 1;

k = (iterations + 1) >> 1;

if (k > r->m_apm_malloclength)
  {
   if ((vp = MAPM_REALLOC(r->m_apm_data, (k + 32))) == NULL)
     {

      M_apm_log_error_msg(M_APM_FATAL, "\'M_apm_sdivide\', Out of memory");
     }

   r->m_apm_malloclength = k + 28;
   r->m_apm_data = (UCHAR *)vp;
  }

M_div_worka->m_apm_exponent = 0;
M_div_workb->m_apm_exponent = 0;

if ((icompare = m_apm_compare(M_div_worka, M_div_workb)) == 0)
  {
   iterations = 1;
   r->m_apm_data[0] = 10;
   nexp++;
  }
else
  {
   if (icompare == 1)
     {
      nexp++;
      M_div_worka->m_apm_exponent += 1;
     }
   else
     {
      M_div_worka->m_apm_exponent += 2;
     }

   indexr = 0;
   m      = 0;

   while (TRUE)
     {

      trial_numer = 10000L * (long)M_div_worka->m_apm_data[0];

      if (M_div_worka->m_apm_datalength >= 5)
        {
         trial_numer += 100 * M_div_worka->m_apm_data[1]
                            + M_div_worka->m_apm_data[2];
	}
      else
        {
         if (M_div_worka->m_apm_datalength >= 3)
           trial_numer += 100 * M_div_worka->m_apm_data[1];
        }

      j = (int)(trial_numer / b0);

      if ((k = 2 - M_div_worka->m_apm_exponent) > 0)
        {
	 while (TRUE)
	   {
	    j /= 10;
	    if (--k == 0)
	      break;
	   }
	}

      if (j == 100)
        j = 99;

      m_apm_set_long(M_div_tmp8, (long)j);
      m_apm_multiply(M_div_tmp7, M_div_tmp8, M_div_workb);

      if (m_apm_compare(M_div_tmp7, M_div_worka) == 1)
        {
	 j--;
         m_apm_subtract(M_div_tmp8, M_div_tmp7, M_div_workb);
         m_apm_copy(M_div_tmp7, M_div_tmp8);
	}

      r->m_apm_data[indexr++] = (UCHAR)j;
      m += 2;

      if (m >= iterations)
        break;

      m_apm_subtract(M_div_tmp9, M_div_worka, M_div_tmp7);

      if (M_div_tmp9->m_apm_sign == 0)
        {
	 iterations = m;
	 break;
	}

      M_div_tmp9->m_apm_exponent += 2;
      m_apm_copy(M_div_worka, M_div_tmp9);
     }
  }

r->m_apm_sign       = sign;
r->m_apm_exponent   = nexp;
r->m_apm_datalength = iterations;

M_apm_normalize(r);
}
