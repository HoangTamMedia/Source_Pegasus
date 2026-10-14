
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_apm.h"

#define  FALSE  0
#define  TRUE   1

extern   void  compute_PI(M_APM, int);

int main(argc, argv)
int argc;  char *argv[];
{
int      ii, dplaces;
char     *buffer;
M_APM    pi_mapm;

if (argc < 2)
  {
   fprintf(stdout,"\nUsage: pi_1  digits\t\t\t\t[Version 1.1]\n");
   fprintf(stdout,
      "       Compute PI to \'digits\' decimal places of accuracy\n");

   exit(4);
  }

pi_mapm = m_apm_init();

dplaces = atoi(argv[1]);

if (dplaces < 2)
  {
   fprintf(stdout,"\nUsage: pi-1  digits \n");
   exit(6);
  }

fprintf(stderr, "\nstart Borwein-Quartic PI calculation \n");

compute_PI(pi_mapm, dplaces);

fprintf(stderr, "Borwein-Quartic PI calculation complete \n\n");

ii = m_apm_significant_digits(pi_mapm) + 12;

if ((buffer = (char *)malloc(ii)) == NULL)
  {
   fprintf(stdout,"PI demo: Out of memory \n");
   exit(8);
  }

m_apm_to_string(buffer, dplaces, pi_mapm);
printf("Borwein PI = [%s] \n",buffer);

m_apm_free(pi_mapm);
free(buffer);

m_apm_free_all_mem();

exit(0);
}

void	compute_PI(outv, places)
M_APM   outv;
int     places;
{
M_APM   tmp1, tmp2, tmp3, c0_5, u0, u1, b0, b1, p0, p1;
int     ct, nn, dplaces, dflag;

tmp1 = m_apm_init();
tmp2 = m_apm_init();
tmp3 = m_apm_init();
c0_5 = m_apm_init();
u0   = m_apm_init();
u1   = m_apm_init();
b0   = m_apm_init();
b1   = m_apm_init();
p0   = m_apm_init();
p1   = m_apm_init();

dplaces = places + 16;

dflag   = FALSE;
ct      = 0;

m_apm_set_string(c0_5, "0.5");

m_apm_sqrt(u0, dplaces, MM_Two);
m_apm_add(p0, u0, MM_Two);

while (TRUE)
  {
   m_apm_sqrt(tmp1, dplaces, u0);
   m_apm_reciprocal(tmp2, dplaces, tmp1);
   m_apm_add(tmp3, tmp1, tmp2);
   m_apm_multiply(u1, c0_5, tmp3);

   m_apm_add(tmp2, MM_One, b0);
   m_apm_multiply(tmp3, tmp1, tmp2);
   m_apm_add(tmp2, b0, u0);
   m_apm_divide(b1, dplaces, tmp3, tmp2);

   m_apm_add(tmp3, MM_One, u1);
   m_apm_multiply(tmp2, b1, tmp3);
   m_apm_multiply(tmp1, p0, tmp2);
   m_apm_add(tmp3, MM_One, b1);
   m_apm_divide(p1, dplaces, tmp1, tmp3);

   if (dflag)
     break;

   m_apm_subtract(tmp2, p0, p1);

   if (++ct >= 4)
     {

      if (m_apm_sign(tmp2) == 0)
        break;
     }

   nn = -m_apm_exponent(tmp2);

   fprintf(stderr, "PI now known to %d digits of accuracy ... \n",(2 * nn));

   if ((4 * nn) >= dplaces)
     dflag = TRUE;

   m_apm_copy(b0, b1);
   m_apm_copy(u0, u1);
   m_apm_copy(p0, p1);
  }

m_apm_round(outv, places, p1);

m_apm_free(p1);
m_apm_free(p0);
m_apm_free(b1);
m_apm_free(b0);
m_apm_free(u1);
m_apm_free(u0);
m_apm_free(c0_5);
m_apm_free(tmp3);
m_apm_free(tmp2);
m_apm_free(tmp1);
}
