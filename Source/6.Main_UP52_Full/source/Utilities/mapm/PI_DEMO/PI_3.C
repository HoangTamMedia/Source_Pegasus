
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_apm.h"

#define  FALSE  0
#define  TRUE   1

#define Mult         m_apm_multiply
#define Add          m_apm_add
#define Subtract     m_apm_subtract
#define Divide       m_apm_divide
#define AssignString m_apm_set_string

extern   void  compute_PI_2(M_APM, int);

void m_apm_square(M_APM x, M_APM y)
{
m_apm_multiply(x, y, y);
}

void m_apm_pow4(M_APM x, M_APM y)
{
m_apm_square(x, y);
m_apm_copy(y, x);
m_apm_square(x, y);
}

void m_apm_root4(M_APM x, int dec_places, M_APM y)
{
m_apm_sqrt(x, dec_places, y);
m_apm_copy(y, x);
m_apm_sqrt(x, dec_places, y);
}

int main(argc, argv)
int argc;  char *argv[];
{
int      ii, dplaces;
char     *buffer;
M_APM    pi_mapm;

if (argc < 2)
  {
   fprintf(stdout,"\nUsage: pi_3  digits\t\t\t\t[Version 1.1]\n");
   fprintf(stdout,
      "       Compute PI to \'digits\' decimal places of accuracy\n");

   exit(4);
  }

pi_mapm = m_apm_init();

dplaces = atoi(argv[1]);

if (dplaces < 2)
  {
   fprintf(stdout,"\nUsage: pi  digits \n");
   exit(6);
  }

fprintf(stderr, "\nstart Borwein-Borwein PI calculation \n");

compute_PI_2(pi_mapm, dplaces);

fprintf(stderr, "Borwein-Borwein PI calculation complete \n\n");

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

void	compute_PI_2(outv, places)
M_APM   outv;
int     places;
{
M_APM   tmp1, tmp2, tmp3, tmp4, a0, a1, b0, b1, MM_Six;
int     kk, ct, nn, dplaces, dflag;

tmp1   = m_apm_init();
tmp2   = m_apm_init();
tmp3   = m_apm_init();
tmp4   = m_apm_init();
a0     = m_apm_init();
a1     = m_apm_init();
b0     = m_apm_init();
b1     = m_apm_init();
MM_Six = m_apm_init();

dplaces = places + 16;

dflag   = FALSE;
ct      = 0;
kk      = 0;

AssignString(MM_Six, "6");

m_apm_sqrt(tmp1, dplaces, MM_Two);
Mult(tmp2, tmp1, MM_Four);
Subtract(a0, MM_Six, tmp2);
Subtract(b0, tmp1, MM_One);

while (TRUE)
  {
   m_apm_pow4(tmp1, b0);
   Subtract(tmp2, MM_One, tmp1);
   m_apm_root4(tmp3, dplaces, tmp2);
   Subtract(tmp1, MM_One, tmp3);
   Add(tmp2, MM_One, tmp3);
   Divide(b1, dplaces, tmp1, tmp2);

   Add(tmp1, MM_One, b1);
   m_apm_pow4(tmp2, tmp1);
   Mult(tmp3, a0, tmp2);

   m_apm_square(tmp1, b1);
   Add(tmp2, tmp1, b1);
   Add(tmp1, tmp2, MM_One);
   Mult(tmp2, tmp1, b1);

   m_apm_integer_pow(tmp1, dplaces,
             MM_Two, (2 * kk + 3));

   Mult(tmp4, tmp1, tmp2);

   Subtract(tmp1, tmp3, tmp4);
   m_apm_round(a1, dplaces, tmp1);

   if (dflag)
     break;

   m_apm_subtract(tmp2, a0, a1);

   if (++ct >= 4)
     {

      if (m_apm_sign(tmp2) == 0)
        break;
     }

   nn = -m_apm_exponent(tmp2);

   fprintf(stderr, "PI now known to %d digits of accuracy ... \n",(4 * nn));

   if ((15 * nn) >= dplaces)
     dflag = TRUE;

   m_apm_copy(b0, b1);
   m_apm_copy(a0, a1);
   kk++;
  }

m_apm_reciprocal(tmp1, dplaces, a1);
m_apm_round(outv, places, tmp1);

m_apm_free(MM_Six);
m_apm_free(b1);
m_apm_free(b0);
m_apm_free(a1);
m_apm_free(a0);
m_apm_free(tmp4);
m_apm_free(tmp3);
m_apm_free(tmp2);
m_apm_free(tmp1);
}
