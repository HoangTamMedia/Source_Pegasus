
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_apm.h"

#define  FALSE  0
#define  TRUE   1

extern   MAPM   compute_PI_2(int);

int main(int argc, char *argv[])
{
int      ii, dplaces;
char     *buffer;
MAPM     pi_mapm;

if (argc < 2)
  {
   fprintf(stdout,"\nUsage: pi_4  digits\t\t\t\t[Version 1.1]\n");
   fprintf(stdout,
      "       Compute PI to \'digits\' decimal places of accuracy\n");

   exit(4);
  }

dplaces = atoi(argv[1]);

if (dplaces < 2)
  {
   fprintf(stdout,"\nUsage: pi-4  digits \n");
   exit(6);
  }

fprintf(stderr, "\nstart Borwein-Borwein PI calculation \n");

pi_mapm = compute_PI_2(dplaces);

fprintf(stderr, "Borwein-Borwein PI calculation complete \n\n");

ii = pi_mapm.significant_digits() + 12;

if ((buffer = (char *)malloc(ii)) == NULL)
  {
   fprintf(stdout,"PI demo: Out of memory \n");
   exit(8);
  }

pi_mapm.toString(buffer, dplaces);
printf("Borwein PI = [%s] \n",buffer);

free(buffer);

exit(0);
}

MAPM	compute_PI_2(int places)
{
MAPM    t1, t2, c2, a0, a1, b0, b1;
int     kk, nn, dplaces, dflag;

dplaces = places + 16;

m_apm_cpp_precision(dplaces);

dflag = FALSE;
kk    = 0;

t1 = sqrt("2.0");
a0 = 6 - 4 * t1;
b0 = t1 - 1;
c2 = 2;

while (TRUE)
  {
   t1 = b0.ipow(4);
   t1 = sqrt(sqrt(1 - t1));
   t1 = t1.round(dplaces);

   b1 = (1 - t1) / (1 + t1);
   b1 = b1.round(dplaces);

   t2 = 1 + b1;
   t2 = t2.ipow(4);
   t2 = t2.round(dplaces);

   t1 = c2.ipow(2 * kk + 3);

   a1 = a0 * t2 - t1 * b1 * (1 + b1 + b1 * b1);
   a1 = a1.round(dplaces);

   if (dflag)
     break;

   t1 = a1 - a0;

   if (kk >= 3)
     {

      if (t1 == 0)
        break;
     }

   nn = -(t1.exponent());

   fprintf(stderr, "PI now known to %d digits of accuracy ... \n",(4 * nn));

   if ((15 * nn) >= dplaces)
     dflag = TRUE;

   b0 = b1;
   a0 = a1;
   kk++;
  }

t1 = 1 / a1;
return(t1.round(places));
}
