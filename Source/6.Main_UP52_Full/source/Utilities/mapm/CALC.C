
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "m_apm.h"

void	 calc_square(M_APM, M_APM);
void	 calc_mod(M_APM, M_APM, M_APM);
void	 calc_pi(M_APM, int);
char 	 *lowercase(char *);

char	 *out_buffer;
char	 in_buffer[8192];

#define  STACK_SIZE 96

M_APM    calc_stack_data[STACK_SIZE];
M_APM    memory_data[36];

#define NUM_OPERATOR_ENTRIES 41

#define	MMM     0x0A01
#define	MiMM    0x0A02
#define	MiM     0x0A03
#define	Mi      0x0A04
#define	MM      0x0A05
#define	MXY     0x0A80
#define	MDUP    0x0A81
#define	M_END   0x0AFF

struct  opstr {
  char  operator[16];
  int	calling_convention;
  void	(*funcname)();
};

struct  opstr  operator_lookup[NUM_OPERATOR_ENTRIES] = {
   {  "+",    MMM,     m_apm_add,               },
   {  "-",    MMM,     m_apm_subtract,          },
   {  "*",    MMM,     m_apm_multiply,          },
   {  "x",    MMM,     m_apm_multiply,          },
   {  "/",    MiMM,    m_apm_divide,            },
   {  "\\",   MMM,     m_apm_integer_divide,  /*   5   */  },
   {  "i",    MMM,     m_apm_integer_divide,    },
   {  "!",    MM,      m_apm_factorial,         },
   {  "s",    MiM,     m_apm_sin,               },
   {  "c",    MiM,     m_apm_cos,               },
   {  "t",    MiM,     m_apm_tan,               },
   {  "q",    MiM,     m_apm_sqrt,              },
   {  "as",   MiM,     m_apm_asin,              },
   {  "ac",   MiM,     m_apm_acos,              },
   {  "at",   MiM,     m_apm_atan,              },
   {  "at2",  MiMM,    m_apm_atan2,             },
   {  "lg",   MiM,     m_apm_log,               },
   {  "lg10", MiM,     m_apm_log10,             },
   {  "e",    MiM,     m_apm_exp,               },
   {  "p",    MiMM,    m_apm_pow,               },
   {  "h",    MM,      m_apm_negate,            },
   {  "n",    MM,      m_apm_negate,            },
   {  "xy",   MXY,     NULL,                    },
   {  "d",    MDUP,    NULL,                    },
   {  "r",    MiM,     m_apm_reciprocal,        },
   {  "x2",   MM,      calc_square,             },
   {  "pi",   Mi,      calc_pi,                 },
   {  "hs",   MiM,     m_apm_sinh,              },
   {  "hc",   MiM,     m_apm_cosh,              },
   {  "ht",   MiM,     m_apm_tanh,              },
   {  "has",  MiM,     m_apm_asinh,             },
   {  "hac",  MiM,     m_apm_acosh,             },
   {  "hat",  MiM,     m_apm_atanh,             },
   {  "b",    MiM,     m_apm_cbrt,              },
   {  "f",    MM,      m_apm_floor,             },
   {  "cl",   MM,      m_apm_ceil,              },
   {  "gcd",  MMM,     m_apm_gcd,               },
   {  "lcm",  MMM,     m_apm_lcm,               },
   {  "mod",  MMM,     calc_mod,                },
   {  "%",    MMM,     calc_mod,                },
   {  "ZZZ",  M_END,   NULL                     }
 };

int main(int argc, char *argv[])
{
void	 (*fp)();
char     ch0, ch1, *cp, version_info[80];
int      ii, ilow, stack_pointer, decimal_places, k,
	 index, len, memory_ref[36];
M_APM    apmtmp;
M_APM    arg1;
M_APM    arg2;

if (argc < 2)
  {
   m_apm_lib_short_version(version_info);

   fprintf(stderr,"calc [-d##] <numbers,operators>\t\t\t\t[Version 2.3]\n");
   fprintf(stderr,
   "      -d : specify decimal places, default = 30\t\t[MAPM Version %s]\n\n",
   	       version_info);

   fprintf(stderr,
"operators :  + : add                           s : sin\n\
             - : subtract                      c : cos\n\
             x : multiply (*)                  t : tan\n\
             / : divide                        q : sqrt\n\
             \\ : integer divide (i)           as : arc-sin\n\
             ! : factorial                    ac : arc-cos\n");

   fprintf(stderr,
"            x2 : x ^ 2                        at : arc-tan\n\
             r : 1 / x                       at2 : arc-tan2 (y,x)\n\
            lg : log                           e : e ^ x\n\
          lg10 : log10                         p : y ^ x\n\
            xy : x<>y (exchange x,y)           n : +/- (negate (h))\n");

   fprintf(stderr,
"            hs : sinh                        has : arc-sinh\n\
            hc : cosh                        hac : arc-cosh\n\
            ht : tanh                        hat : arc-tanh\n\
             b : cube root                     d : dup (HP-Enter)\n");

   fprintf(stderr,
"             f : floor                        cl : ceil\n\
           gcd : GCD                         lcm : LCM\n\
            s# : save memory, # = 0-9,a-z     r# : recall memory, # = 0-9,a-z\n\
            pi : 3.14159..                   mod : modulus (%%)\n");

   exit(2);
  }

apmtmp = m_apm_init();

for (k=0; k < STACK_SIZE; k++)
  calc_stack_data[k] = m_apm_init();

for (k=0; k < 36; k++)
  {
   memory_data[k] = m_apm_init();
   memory_ref[k]  = 0;
  }

stack_pointer  = 16;
decimal_places = 30;
ilow           = 1;

strcpy(in_buffer, argv[1]);
lowercase(in_buffer);
if ((cp = strstr(in_buffer,"-d")) != NULL)
  {
   ilow++;
   cp += 2;
   decimal_places = atoi(cp);
   if (decimal_places < 2)
     decimal_places = 2;
  }

for (ii=ilow; ii < argc; ii++)
  {
   strcpy(in_buffer, argv[ii]);
   lowercase(in_buffer);
   k = 0;

   while (1)
     {

      if (k == 0)
        {
         if (isdigit((int)*in_buffer))
           {
	    stack_pointer++;
	    m_apm_set_string(calc_stack_data[stack_pointer], in_buffer);
	    break;
	   }
	}

      if (operator_lookup[k].calling_convention == M_END)
        {

	 len = strlen(in_buffer);
	 ch0 = in_buffer[0];
	 ch1 = in_buffer[1];

	 if (ch0 == 's' || ch0 == 'r')
	   {

            index = -1;

            if (ch1 >= '0' && ch1 <= '9')
              index = (int)ch1 - '0';

            if (ch1 >= 'a' && ch1 <= 'z')
              index = (int)ch1 + 10 - 'a';

            if (len == 2 && index >= 0)
	      {
	       if (ch0 == 'r')
	         {
		  if (memory_ref[index] != 0)
		    {
	             stack_pointer++;
	             arg1 = memory_data[index];
	             m_apm_copy(calc_stack_data[stack_pointer], arg1);
		    }
		  else
		    {
               fprintf(stderr,
	"Warning!, Using un-initialized memory location : \'%c\'\n",ch1);
		    }
		 }
               else
	         {
	          arg1 = memory_data[index];
	          m_apm_copy(arg1, calc_stack_data[stack_pointer]);
		  memory_ref[index] = 1;
		 }
	      }
            else
	      {
               fprintf(stderr,
	"Error!, Memory command error : Syntax  s[0-9,a-z] or r[0-9,a-z]\n");
	       exit(4);
              }
	   }
         else
	   {
	    stack_pointer++;
	    m_apm_set_string(calc_stack_data[stack_pointer], in_buffer);
	   }

	 break;
	}

      if (strcmp(operator_lookup[k].operator, in_buffer) == 0)
        {

	 if (operator_lookup[k].calling_convention == Mi)
	   {
            fp = operator_lookup[k].funcname;
            (*fp)(apmtmp, (decimal_places + 6));
	    m_apm_copy(calc_stack_data[++stack_pointer], apmtmp);
	   }

	 if (operator_lookup[k].calling_convention == MXY)
	   {
	    arg2 = calc_stack_data[stack_pointer - 1];
	    arg1 = calc_stack_data[stack_pointer];

	    m_apm_copy(apmtmp, arg1);
	    m_apm_copy(arg1, arg2);
	    m_apm_copy(arg2, apmtmp);
	   }

	 if (operator_lookup[k].calling_convention == MDUP)
	   {
	    arg1 = calc_stack_data[stack_pointer];
	    m_apm_copy(calc_stack_data[++stack_pointer], arg1);
	   }

	 if (operator_lookup[k].calling_convention == MMM)
	   {
            fp   = operator_lookup[k].funcname;
	    arg2 = calc_stack_data[stack_pointer];
	    arg1 = calc_stack_data[--stack_pointer];
            (*fp)(apmtmp, arg1, arg2);
	    m_apm_copy(arg1, apmtmp);
	   }

	 if (operator_lookup[k].calling_convention == MiMM)
	   {
            fp   = operator_lookup[k].funcname;
	    arg2 = calc_stack_data[stack_pointer];
	    arg1 = calc_stack_data[--stack_pointer];
            (*fp)(apmtmp, (decimal_places + 6), arg1, arg2);
	    m_apm_copy(arg1, apmtmp);
	   }

	 if (operator_lookup[k].calling_convention == MM)
	   {
            fp   = operator_lookup[k].funcname;
	    arg1 = calc_stack_data[stack_pointer];
            (*fp)(apmtmp, arg1);
	    m_apm_copy(arg1, apmtmp);
	   }

	 if (operator_lookup[k].calling_convention == MiM)
	   {
            fp   = operator_lookup[k].funcname;
	    arg1 = calc_stack_data[stack_pointer];
            (*fp)(apmtmp, (decimal_places + 6), arg1);
	    m_apm_copy(arg1, apmtmp);
	   }

	 break;
	}

      k++;
     }
  }

if (stack_pointer <= 16)
  {
   fprintf(stderr,"Warning!, stack under-flow, result is unreliable\n");
  }

arg1 = calc_stack_data[stack_pointer];
m_apm_round(apmtmp, decimal_places, arg1);

k = m_apm_exponent(apmtmp);
if (k >= -4 && k <= 6)
  {
   out_buffer = m_apm_to_fixpt_stringexp(-1, apmtmp, '.', 0, 0);
  }
else
  {
   out_buffer = (char *)malloc(decimal_places + 16);

   if (out_buffer != NULL)
     {
      m_apm_to_string(out_buffer, -1, apmtmp);
     }
  }

if (out_buffer == NULL)
  {
   fprintf(stderr,"calc: out of memory\n");
   exit(4);
  }

printf("%s\n",out_buffer);

for (k=0; k < STACK_SIZE; k++)
  m_apm_free(calc_stack_data[k]);

for (k=0; k < 36; k++)
  m_apm_free(memory_data[k]);

m_apm_free(apmtmp);
m_apm_free_all_mem();

free(out_buffer);

exit(0);
}

void	 calc_square(M_APM rr, M_APM xx)
{
m_apm_multiply(rr, xx, xx);
}

void	 calc_mod(M_APM mm, M_APM aa, M_APM bb)
{
M_APM    qq;

qq = m_apm_init();

m_apm_integer_div_rem(qq, mm, aa, bb);

m_apm_free(qq);
}

void	 calc_pi(M_APM rr, int p)
{
M_APM    aa;

if (p > 115)
  {
   aa = m_apm_init();

   m_apm_asin(aa, (p + 2), MM_One);
   m_apm_add(rr, aa, aa);

   m_apm_free(aa);
  }
else
  m_apm_round(rr, p, MM_PI);
}

char	*lowercase(char *s)
{
char    *p;

p = s;
while (1)
  {
   if (*p >= 'A' && *p <= 'Z')  *p += 'a' - 'A';
   if (*p++ == 0)  break;
  }
return(s);
}
