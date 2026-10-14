
#include "m_apm_lc.h"

#define NDIM 32

void	m_apm_factorial(M_APM moutput, M_APM minput)
{
int     ii, nmul, ndigits, nd, jj, kk, mm, ct;
M_APM   array[NDIM];
M_APM   iprod1, iprod2, tmp1, tmp2;

if (m_apm_compare(minput, MM_One) <= 0)
  {
   m_apm_copy(moutput, MM_One);
   return;
  }

ct       = 0;
mm       = NDIM - 2;
ndigits  = 256;
nd       = ndigits - 20;
tmp1     = m_apm_init();
tmp2     = m_apm_init();
iprod1   = m_apm_init();
iprod2   = m_apm_init();
array[0] = m_apm_init();

m_apm_copy(tmp2, minput);

while (TRUE)
  {
   m_apm_copy(iprod1, MM_One);

   while (TRUE)
     {
      m_apm_multiply(iprod2, iprod1, tmp2);

      m_apm_subtract(tmp1, tmp2, MM_One);

      m_apm_multiply(iprod1, iprod2, tmp1);

      if (m_apm_compare(tmp1, MM_Two) <= 0)
        goto PHASE2;

      m_apm_subtract(tmp2, tmp1, MM_One);

      if (iprod1->m_apm_datalength > nd)
        break;
     }

   if (ct == (NDIM - 1))
     {

      m_apm_copy(tmp1, array[mm]);
      m_apm_multiply(array[mm], iprod1, tmp1);

      if (mm == 0)
        {
         mm = NDIM - 2;
	 ndigits = ndigits << 1;
         nd = ndigits - 20;
	}
      else
         mm--;
     }
   else
     {

      m_apm_copy(array[ct], iprod1);
      array[++ct] = m_apm_init();
     }
  }

PHASE2:

m_apm_copy(array[ct], iprod1);

kk = ct;

while (kk != 0)
  {
   ii = 0;
   jj = 0;
   nmul = (kk + 1) >> 1;

   while (TRUE)
     {

      if (ii == 0)
        {
         m_apm_copy(tmp1, array[ii]);
         m_apm_multiply(array[jj], tmp1, array[ii+1]);
        }
      else
         m_apm_multiply(array[jj], array[ii], array[ii+1]);

      if (++jj == nmul)
        break;

      ii += 2;
     }

   if ((kk & 1) == 0)
     {
      jj = kk >> 1;
      m_apm_copy(array[jj], array[kk]);
     }

   kk = kk >> 1;
  }

m_apm_copy(moutput, array[0]);

for (ii=0; ii <= ct; ii++)
  {
   m_apm_free(array[ii]);
  }

m_apm_free(tmp1);
m_apm_free(tmp2);
m_apm_free(iprod1);
m_apm_free(iprod2);
}
