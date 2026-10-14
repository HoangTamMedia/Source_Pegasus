
#include "m_apm_lc.h"
#include <ctype.h>

char	*m_apm_to_fixpt_stringexp(int dplaces, M_APM atmp,
				  char ch_radx, char ch_sep, int ct_sep)
{
int	places, xp, dl, ii;
char	*cpr;

places = dplaces;

dl = atmp->m_apm_datalength;
xp = atmp->m_apm_exponent;

if (places < 0)
  {
   if (xp < 0)
      ii = dl - xp;
   else
     {
      if (dl > xp)
        ii = dl;
      else
        ii = xp;
     }
  }
else
  {
   ii = places;

   if (xp > 0)
     ii += xp;
  }

if (ct_sep != 0 && ch_sep != 0 && xp > 0)
  ii += xp / ct_sep;

if ((cpr = (char *)MAPM_MALLOC((ii + 32) * sizeof(char))) == NULL)
  return(NULL);

m_apm_to_fixpt_stringex(cpr,places,atmp,ch_radx,ch_sep,ct_sep);

return(cpr);
}

void	m_apm_to_fixpt_stringex(char *s, int dplaces, M_APM atmp,
				char ch_radix, char ch_sep, int count_sep)
{
M_APM   btmp;
char    ch, *cpd, *cps;
int	ii, jj, kk, ct, dl, xp, no_sep_flg, places;

btmp       = M_get_stack_var();
places     = dplaces;
cpd        = s;
no_sep_flg = FALSE;

m_apm_absolute_value(btmp, atmp);

if (ch_sep == 0 || count_sep == 0)
  no_sep_flg = TRUE;

dl = btmp->m_apm_datalength;
xp = btmp->m_apm_exponent;

if (places < 0)
  {
   if (xp < 0)
      ii = dl - xp;
   else
     {
      if (dl > xp)
        ii = dl;
      else
        ii = xp;
     }
  }
else
  {
   ii = places;

   if (xp > 0)
     ii += xp;
  }

if ((cps = (char *)MAPM_MALLOC((ii + 32) * sizeof(char))) == NULL)
  {

   M_apm_log_error_msg(M_APM_FATAL,
                       "\'m_apm_to_fixpt_stringex\', Out of memory");
  }

m_apm_to_fixpt_string(cps, places, btmp);

if (atmp->m_apm_sign == -1)
  {
   kk = 0;
   jj = 0;

   while (TRUE)
     {
      ch = cps[kk++];
      if ((ch == '\0') || (jj != 0))
        break;

      if (isdigit((int)ch))
        {
	 if (ch != '0')
	   jj = 1;
	}
     }

   if (jj)
     *cpd++ = '-';
  }

ct = M_strposition(cps, ".");

if (ct == -1)
  {
   strcat(cps, ".");
   ct = M_strposition(cps, ".");
  }

if (places == 0)
  cps[ct] = '\0';
else
  cps[ct] = ch_radix;

if (ct <= count_sep)
  no_sep_flg = TRUE;

if (no_sep_flg)
  {
   strcpy(cpd, cps);
  }
else
  {
   jj = 0;
   kk = count_sep;
   ii = ct / count_sep;

   if ((ii = ct - ii * count_sep) == 0)
     ii = count_sep;

   while (TRUE)
     {
      *cpd++ = cps[jj++];

      if (--ii == 0)
        break;
     }

   while (TRUE)
     {
      if (kk == count_sep)
        {
	 if (jj != ct)
	   {
            *cpd++ = ch_sep;
	    kk = 0;
	   }
	}

      if ((*cpd++ = cps[jj++]) == '\0')
        break;

      kk++;
     }
  }

MAPM_FREE(cps);
M_restore_stack(1);
}

void	m_apm_to_fixpt_string(char *ss, int dplaces, M_APM mtmp)
{
M_APM   ctmp;
void	*vp;
int	places, i2, ii, jj, kk, xp, dl, numb;
UCHAR   *ucp, numdiv, numrem;
char	*cpw, *cpd, sbuf[128];

ctmp   = M_get_stack_var();
vp     = NULL;
cpd    = ss;
places = dplaces;

if (places == 0)
  {
   if (mtmp->m_apm_sign >= 0)
     m_apm_add(ctmp, mtmp, MM_0_5);
   else
     m_apm_subtract(ctmp, mtmp, MM_0_5);

   m_apm_to_integer_string(cpd, ctmp);

   M_restore_stack(1);
   return;
  }

if (places > 0)
  M_apm_round_fixpt(ctmp, places, mtmp);
else
  m_apm_copy(ctmp, mtmp);

if (ctmp->m_apm_sign == 0)
  {
   if (places < 0)
     {
      cpd[0] = '0';
      cpd[1] = '.';
      cpd[2] = '0';
      cpd[3] = '\0';
     }
   else
     {
      memset(cpd, '0', (places + 2));
      cpd[1] = '.';
      cpd[places + 2] = '\0';
     }

   M_restore_stack(1);
   return;
  }

xp   = ctmp->m_apm_exponent;
dl   = ctmp->m_apm_datalength;
numb = (dl + 1) >> 1;

if (places < 0)
  {
   if (dl > xp)
     jj = dl + 16;
   else
     jj = xp + 16;
  }
else
  {
   jj = places + 16;

   if (xp > 0)
     jj += xp;
  }

if (jj > 112)
  {
   if ((vp = (void *)MAPM_MALLOC((jj + 16) * sizeof(char))) == NULL)
     {

      M_apm_log_error_msg(M_APM_FATAL,
                          "\'m_apm_to_fixpt_string\', Out of memory");
     }

   cpw = (char *)vp;
  }
else
  {
   cpw = sbuf;
  }

if (ctmp->m_apm_sign == -1)
  {
   *cpd++ = '-';
  }

ucp = ctmp->m_apm_data;
ii  = 0;

while (TRUE)
  {
   M_get_div_rem_10((int)(*ucp++), &numdiv, &numrem);

   cpw[ii++] = numdiv + '0';
   cpw[ii++] = numrem + '0';

   if (--numb == 0)
     break;
  }

i2 = ii;

if (places < 0)
  {
   places = dl - xp;

   if (places < 1)
     places = 1;
  }

kk = xp + places + 2 - ii;

if (kk > 0)
  memset(&cpw[ii], '0', kk);

if (xp > 0)
  {
   ii = xp + places + 1;
   jj = 0;

   for (kk=0; kk < ii; kk++)
     {
      if (kk == xp)
        cpd[jj++] = '.';

      cpd[jj++] = cpw[kk];
     }

   cpd[ii] = '\0';
  }
else
  {
   jj = 2 - xp;
   ii = 2 + places;
   memset(cpd, '0', (ii + 1));
   cpd[1] = '.';

   for (kk=0; kk < i2; kk++)
     {
      cpd[jj++] = cpw[kk];
     }

   cpd[ii] = '\0';
  }

if (vp != NULL)
  MAPM_FREE(vp);

M_restore_stack(1);
}

void	M_apm_round_fixpt(M_APM btmp, int places, M_APM atmp)
{
int	xp, ii;

xp = atmp->m_apm_exponent;
ii = xp + places - 1;

M_set_to_zero(btmp);

if (ii >= 0)
  {
   m_apm_round(btmp, ii, atmp);
  }
else
  {
   if (ii == -1)
     {
      if (atmp->m_apm_data[0] >= 50)
        {
         m_apm_copy(btmp, atmp);
	 btmp->m_apm_data[0] = 10;
	 btmp->m_apm_exponent += 1;
	 btmp->m_apm_datalength = 1;
	 M_apm_normalize(btmp);
	}
     }
  }
}
