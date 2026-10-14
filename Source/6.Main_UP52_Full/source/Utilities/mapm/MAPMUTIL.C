
#include "m_apm_lc.h"

static  UCHAR	*M_mul_div = NULL;
static  UCHAR   *M_mul_rem = NULL;

static  UCHAR   M_mul_div_10[100];
static	UCHAR   M_mul_rem_10[100];

static	int	M_util_firsttime = TRUE;
static	int     M_firsttime3 = TRUE;

static	M_APM	M_work_0_5;

static  char    *M_init_error_msg = "\'m_apm_init\', Out of memory";

M_APM	m_apm_init()
{
M_APM	atmp;

if (M_firsttime3)
  {
   M_firsttime3 = FALSE;
   M_init_util_data();
   M_init_trig_globals();
  }

if ((atmp = (M_APM)MAPM_MALLOC(sizeof(M_APM_struct))) == NULL)
  {

   M_apm_log_error_msg(M_APM_FATAL, M_init_error_msg);
  }

atmp->m_apm_id           = M_APM_IDENT;
atmp->m_apm_malloclength = 80;
atmp->m_apm_datalength   = 1;
atmp->m_apm_refcount     = 1;
atmp->m_apm_exponent     = 0;
atmp->m_apm_sign         = 0;

if ((atmp->m_apm_data = (UCHAR *)MAPM_MALLOC(84)) == NULL)
  {

   M_apm_log_error_msg(M_APM_FATAL, M_init_error_msg);
  }

atmp->m_apm_data[0] = 0;
return(atmp);
}

void	m_apm_free(M_APM atmp)
{
if (atmp->m_apm_id == M_APM_IDENT)
  {
   atmp->m_apm_id = 0x0FFFFFF0L;
   MAPM_FREE(atmp->m_apm_data);
   MAPM_FREE(atmp);
  }
else
  {
   M_apm_log_error_msg(M_APM_RETURN, "\'m_apm_free\', Invalid M_APM variable");
  }
}

void	M_free_all_util()
{
if (M_util_firsttime == FALSE)
  {
   m_apm_free(M_work_0_5);
   M_util_firsttime = TRUE;
  }

if (M_firsttime3 == FALSE)
  {
   MAPM_FREE(M_mul_div);
   MAPM_FREE(M_mul_rem);

   M_mul_div    = NULL;
   M_mul_rem    = NULL;
   M_firsttime3 = TRUE;
  }
}

int 	M_get_sizeof_int()
{
return(sizeof(int));
}

void	M_init_util_data()
{
int	k;
UCHAR   ndiv, nrem;

if (M_mul_div != NULL)
  return;

M_mul_div = (UCHAR *)MAPM_MALLOC(10000 * sizeof(UCHAR));
M_mul_rem = (UCHAR *)MAPM_MALLOC(10000 * sizeof(UCHAR));

if (M_mul_div == NULL || M_mul_rem == NULL)
  {

   M_apm_log_error_msg(M_APM_FATAL, "\'M_init_util_data\', Out of memory");
  }

ndiv = 0;
nrem = 0;

for (k=0; k < 100; k++)
  {
   M_mul_div_10[k] = ndiv;
   M_mul_rem_10[k] = nrem;

   if (++nrem == 10)
     {
      nrem = 0;
      ndiv++;
     }
  }

ndiv = 0;
nrem = 0;

for (k=0; k < 10000; k++)
  {
   M_mul_div[k] = ndiv;
   M_mul_rem[k] = nrem;

   if (++nrem == 100)
     {
      nrem = 0;
      ndiv++;
     }
  }
}

void	M_get_div_rem_addr(UCHAR **ndivp, UCHAR **nremp)
{
*ndivp = M_mul_div;
*nremp = M_mul_rem;
}

void	M_get_div_rem(int tbl_lookup, UCHAR *ndiv, UCHAR *nrem)
{
*ndiv = M_mul_div[tbl_lookup];
*nrem = M_mul_rem[tbl_lookup];
}

void	M_get_div_rem_10(int tbl_lookup, UCHAR *ndiv, UCHAR *nrem)
{
*ndiv = M_mul_div_10[tbl_lookup];
*nrem = M_mul_rem_10[tbl_lookup];
}

void	m_apm_round(M_APM btmp, int places, M_APM atmp)
{
int	ii;

if (M_util_firsttime)
  {
   M_util_firsttime = FALSE;

   M_work_0_5 = m_apm_init();
   m_apm_set_string(M_work_0_5, "5");
  }

ii = places + 1;

if (atmp->m_apm_datalength <= ii)
  {
   m_apm_copy(btmp,atmp);
   return;
  }

M_work_0_5->m_apm_exponent = atmp->m_apm_exponent - ii;

if (atmp->m_apm_sign > 0)
  m_apm_add(btmp, atmp, M_work_0_5);
else
  m_apm_subtract(btmp, atmp, M_work_0_5);

btmp->m_apm_datalength = ii;
M_apm_normalize(btmp);
}

void	M_apm_normalize(M_APM atmp)
{
int	i, index, datalength, exponent;
UCHAR   *ucp, numdiv, numrem, numrem2;

if (atmp->m_apm_sign == 0)
  return;

datalength = atmp->m_apm_datalength;
exponent   = atmp->m_apm_exponent;

M_apm_pad(atmp, (datalength + 3));

while (TRUE)
  {
   M_get_div_rem_10((int)atmp->m_apm_data[0], &numdiv, &numrem);

   if (numdiv >= 1)
     break;

   index = (datalength + 1) >> 1;

   if (numrem == 0)
     {
      i = 0;
      ucp = atmp->m_apm_data;

      while (TRUE)
        {
	 if (*ucp != 0)
	   break;

         ucp++;
	 i++;
	}

      memmove(atmp->m_apm_data, ucp, (index + 1 - i));
      datalength -= 2 * i;
      exponent -= 2 * i;
     }
   else
     {
      for (i=0; i < index; i++)
        {
         M_get_div_rem_10((int)atmp->m_apm_data[i+1], &numdiv, &numrem2);
         atmp->m_apm_data[i] = 10 * numrem + numdiv;
	 numrem = numrem2;
        }

      datalength--;
      exponent--;
     }
  }

while (TRUE)
  {
   index = ((datalength + 1) >> 1) - 1;

   if ((datalength & 1) == 0)
     {
      ucp = atmp->m_apm_data + index;
      if (*ucp == 0)
        {
	 while (TRUE)
	   {
	    datalength -= 2;
	    index--;
	    ucp--;

	    if (*ucp != 0)
	      break;
	   }
	}
     }

   M_get_div_rem_10((int)atmp->m_apm_data[index], &numdiv, &numrem);

   if (numrem != 0)
     break;

   if ((datalength & 1) != 0)
     {
      if (numdiv != 0)
        break;
     }

   if (datalength == 1)
     {
      atmp->m_apm_sign = 0;
      exponent = 0;
      break;
     }

   datalength--;
  }

atmp->m_apm_datalength = datalength;
atmp->m_apm_exponent   = exponent;
}

void	M_apm_scale(M_APM ctmp, int count)
{
int	ii, numb, ct;
UCHAR	*chp, numdiv, numdiv2, numrem;
void	*vp;

ct = count;

ii = (ctmp->m_apm_datalength + ct + 1) >> 1;
if (ii > ctmp->m_apm_malloclength)
  {
   if ((vp = MAPM_REALLOC(ctmp->m_apm_data, (ii + 32))) == NULL)
     {

      M_apm_log_error_msg(M_APM_FATAL, "\'M_apm_scale\', Out of memory");
     }

   ctmp->m_apm_malloclength = ii + 28;
   ctmp->m_apm_data = (UCHAR *)vp;
  }

if ((ct & 1) != 0)
  {
   ct--;
   chp = ctmp->m_apm_data;
   ii  = ((ctmp->m_apm_datalength + 1) >> 1) - 1;

   if ((ctmp->m_apm_datalength & 1) == 0)
     {

      numdiv = 0;

      while (TRUE)
        {
         M_get_div_rem_10((int)chp[ii], &numdiv2, &numrem);

	 chp[ii + 1] = 10 * numrem + numdiv;
	 numdiv = numdiv2;

	 if (ii == 0)
	   break;

         ii--;
	}

      chp[0] = numdiv2;
     }
   else
     {

      M_get_div_rem_10((int)chp[ii], &numdiv2, &numrem);

      if (ii == 0)
        {
         chp[0] = numdiv2;
        }
      else
        {
         while (TRUE)
           {
            M_get_div_rem_10((int)chp[ii - 1], &numdiv, &numrem);

	    chp[ii] = 10 * numrem + numdiv2;
	    numdiv2 = numdiv;

	    if (--ii == 0)
	      break;
	   }

         chp[0] = numdiv;
        }
     }

   ctmp->m_apm_exponent++;
   ctmp->m_apm_datalength++;
  }

if (ct > 0)
  {
   numb = (ctmp->m_apm_datalength + 1) >> 1;
   ii   = ct >> 1;

   memmove((ctmp->m_apm_data + ii), ctmp->m_apm_data, numb);
   memset(ctmp->m_apm_data, 0, ii);

   ctmp->m_apm_datalength += ct;
   ctmp->m_apm_exponent += ct;
  }
}

void	M_apm_pad(M_APM ctmp, int new_length)
{
int	num1, numb, ct;
UCHAR	numdiv, numrem;
void	*vp;

ct = new_length;
if (ctmp->m_apm_datalength >= ct)
  return;

numb = (ct + 1) >> 1;
if (numb > ctmp->m_apm_malloclength)
  {
   if ((vp = MAPM_REALLOC(ctmp->m_apm_data, (numb + 32))) == NULL)
     {

      M_apm_log_error_msg(M_APM_FATAL, "\'M_apm_pad\', Out of memory");
     }

   ctmp->m_apm_malloclength = numb + 28;
   ctmp->m_apm_data = (UCHAR *)vp;
  }

num1 = (ctmp->m_apm_datalength + 1) >> 1;

if ((ctmp->m_apm_datalength & 1) != 0)
  {
   M_get_div_rem_10((int)ctmp->m_apm_data[num1 - 1], &numdiv, &numrem);
   ctmp->m_apm_data[num1 - 1] = 10 * numdiv;
  }

memset((ctmp->m_apm_data + num1), 0, (numb - num1));
ctmp->m_apm_datalength = ct;
}
