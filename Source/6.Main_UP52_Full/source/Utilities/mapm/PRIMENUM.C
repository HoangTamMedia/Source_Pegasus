
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "m_apm.h"

extern  int     is_number_prime(M_APM);
extern  void    init_working_mapm(void);
extern  void    free_working_mapm(void);

#define FALSE 0
#define TRUE 1

char	buffer[2048];

static  M_APM   M_limit, M_digit, M_quot, M_rem, M_tmp0, M_tmp1;

int main(int argc, char *argv[])
{
char	 version_info[80];
int      ct;

M_APM    aa_mapm;
M_APM    bb_mapm;
M_APM    cc_mapm;
M_APM    dd_mapm;

if (argc < 2)
  {
   m_apm_lib_short_version(version_info);

   fprintf(stdout,
      "Usage: primenum number\t\t\t[Version 1.4, MAPM Version %s]\n",
      	      version_info);
   fprintf(stdout,
      "       find the first 10 prime numbers starting with \'number\'\n");

   exit(4);
  }

aa_mapm = m_apm_init();
bb_mapm = m_apm_init();
cc_mapm = m_apm_init();
dd_mapm = m_apm_init();

init_working_mapm();

m_apm_set_string(dd_mapm, argv[1]);

if (m_apm_compare(dd_mapm, MM_Three) == -1)
  {
   m_apm_copy(dd_mapm, MM_Three);
  }

m_apm_integer_divide(aa_mapm, dd_mapm, MM_Two);
m_apm_multiply(bb_mapm, MM_Two, aa_mapm);
m_apm_add(aa_mapm, MM_One, bb_mapm);

ct = 0;

while (TRUE)
  {
   if (is_number_prime(aa_mapm))
     {
      m_apm_to_integer_string(buffer, aa_mapm);
      fprintf(stdout,"%s\n",buffer);

      if (++ct == 10)
        break;
     }

   m_apm_add(cc_mapm, MM_Two, aa_mapm);
   m_apm_copy(aa_mapm, cc_mapm);
  }

free_working_mapm();

m_apm_free(aa_mapm);
m_apm_free(bb_mapm);
m_apm_free(cc_mapm);
m_apm_free(dd_mapm);

m_apm_free_all_mem();

exit(0);
}

int     is_number_prime(M_APM input)
{
int     ii, ret, index;
char    sbuf[32];

static  char  incr_table[48] = {
        2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 6, 6, 2, 6, 4, 2,
        6, 4, 6, 8, 4, 2, 4, 2, 4, 8, 6, 4, 6, 2, 4, 6,
        2, 6, 6, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 10, 2, 10 };

if (m_apm_compare(input, MM_Ten) <= 0)
  {
   m_apm_to_integer_string(sbuf, input);
   ii = atoi(sbuf);

   if (ii == 2 || ii == 3 || ii == 5 || ii == 7)
     return(TRUE);
   else
     return(FALSE);
  }

ret   = FALSE;
index = 0;

m_apm_integer_div_rem(M_quot, M_rem, input, MM_Three);
if (m_apm_sign(M_rem) == 0)
  return(ret);

m_apm_integer_div_rem(M_quot, M_rem, input, MM_Five);
if (m_apm_sign(M_rem) == 0)
  return(ret);

m_apm_set_long(M_digit, 7L);
m_apm_integer_div_rem(M_quot, M_rem, input, M_digit);
if (m_apm_sign(M_rem) == 0)
  return(ret);

ii = m_apm_exponent(input) + 16;

m_apm_sqrt(M_tmp1, ii, input);
m_apm_add(M_limit, MM_Two, M_tmp1);

m_apm_set_long(M_digit, 11L);

while (TRUE)
  {
   if (m_apm_compare(M_digit, M_limit) >= 0)
     {
      ret = TRUE;
      break;
     }

   m_apm_integer_div_rem(M_quot, M_rem, input, M_digit);

   if (m_apm_sign(M_rem) == 0)
     break;

   m_apm_set_long(M_tmp1, (long)incr_table[index]);
   m_apm_add(M_tmp0, M_digit, M_tmp1);
   m_apm_copy(M_digit, M_tmp0);

   if (++index == 48)
     index = 0;
  }

return(ret);
}

void    init_working_mapm()
{
M_limit = m_apm_init();
M_digit = m_apm_init();
M_quot  = m_apm_init();
M_rem   = m_apm_init();
M_tmp0  = m_apm_init();
M_tmp1  = m_apm_init();
}

void    free_working_mapm()
{
m_apm_free(M_limit);
m_apm_free(M_digit);
m_apm_free(M_quot);
m_apm_free(M_rem);
m_apm_free(M_tmp0);
m_apm_free(M_tmp1);
}
