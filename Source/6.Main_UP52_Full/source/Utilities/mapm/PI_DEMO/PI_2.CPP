
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_apm.h"

#define  FALSE  0
#define  TRUE   1

extern   MAPM  compute_PI(int);

int main(int argc, char **argv)
{
int      ii, dplaces;
char     *buffer;
MAPM     pi_mapm;

if (argc < 2)
  {
   fprintf(stdout,"\nUsage: pi_2  digits\t\t\t\t[Version 1.1]\n");
   fprintf(stdout,
      "       Compute PI to \'digits\' decimal places of accuracy\n");

   exit(4);
  }

dplaces = atoi(argv[1]);

if (dplaces < 2)
  {
   fprintf(stdout,"\nUsage: pi-2  digits \n");
   exit(6);
  }

fprintf(stderr, "\nstart Borwein-Quartic PI calculation \n");

pi_mapm = compute_PI(dplaces);

fprintf(stderr, "Borwein-Quartic PI calculation complete \n\n");

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

MAPM	compute_PI(int places)
{
MAPM    t1, c0_5, u0, u1, b0, b1, p0, p1;
int     ct, nn, dplaces, dflag;

dplaces = places + 16;

m_apm_cpp_precision(dplaces);

dflag = FALSE;
ct    = 0;

c0_5  = "0.5";

u0    = sqrt("2");
b0    = 0;
p0    = 2 + u0;

while (TRUE)
  {

   t1 = sqrt(u0);
   u1 = c0_5 * (t1 + 1 / t1);

   b1 = (t1 * (1 + b0)) / (u0 + b0);

   p1 = (p0 * b1 * (1 + u1)) / (1 + b1);

   if (dflag)
     break;

   t1 = p1 - p0;

   if (++ct >= 4)
     {

      if (t1 == 0)
        break;
     }

   nn = -(t1.exponent());

   fprintf(stderr, "PI now known to %d digits of accuracy ... \n",(2 * nn));

   if ((4 * nn) >= dplaces)
     dflag = TRUE;

   b0 = b1.round(dplaces);
   u0 = u1.round(dplaces);
   p0 = p1.round(dplaces);
  }

return (p1.round(places));
}
