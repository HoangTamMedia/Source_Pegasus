
#include "m_apm_lc.h"

void	m_apm_to_integer_string(char *s, M_APM mtmp)
{
void    *vp;
UCHAR	*ucp, numdiv, numrem;
char	*cp, *p, sbuf[128];
int	ct, dl, numb, ii;

vp = NULL;
ct = mtmp->m_apm_exponent;
dl = mtmp->m_apm_datalength;

if (ct <= 0 || mtmp->m_apm_sign == 0)
  {
   s[0] = '0';
   s[1] = '\0';
   return;
  }

if (ct > 112)
  {
   if ((vp = (void *)MAPM_MALLOC((ct + 32) * sizeof(char))) == NULL)
     {

      M_apm_log_error_msg(M_APM_FATAL,
                          "\'m_apm_to_integer_string\', Out of memory");
     }

   cp = (char *)vp;
  }
else
  {
   cp = sbuf;
  }

p  = cp;
ii = 0;

if (mtmp->m_apm_sign == -1)
  {
   ii = 1;
   *p++ = '-';
  }

if (ct > dl)
  numb = (dl + 1) >> 1;
else
  numb = (ct + 1) >> 1;

ucp = mtmp->m_apm_data;

while (TRUE)
  {
   M_get_div_rem_10((int)(*ucp++), &numdiv, &numrem);

   *p++ = numdiv + '0';
   *p++ = numrem + '0';

   if (--numb == 0)
     break;
  }

if (ct > dl)
  memset(p, '0', (ct + 1 - dl));

cp[ct + ii] = '\0';
strcpy(s, cp);

if (vp != NULL)
  MAPM_FREE(vp);
}
