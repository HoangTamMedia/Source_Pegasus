
#include "m_apm_lc.h"

void	M_get_sqrt_guess(M_APM r, M_APM a)
{
char	buf[48];
double  dd;

m_apm_to_string(buf, 15, a);
dd = atof(buf);
m_apm_set_double(r, (1.0 / sqrt(dd)));
}

void	M_get_cbrt_guess(M_APM r, M_APM a)
{
char	buf[48];
double  dd;

m_apm_to_string(buf, 15, a);
dd = atof(buf);
dd = log(dd) / 3.0;
m_apm_set_double(r, (1.00001 / exp(dd)));
}

void	M_get_log_guess(M_APM r, M_APM a)
{
char	buf[48];
double  dd;

m_apm_to_string(buf, 15, a);
dd = atof(buf);
m_apm_set_double(r, (1.00001 * log(dd)));
}

void	M_get_asin_guess(M_APM r, M_APM a)
{
char	buf[48];
double  dd;

m_apm_to_string(buf, 15, a);
dd = atof(buf);
m_apm_set_double(r, (1.00001 * asin(dd)));
}

void	M_get_acos_guess(M_APM r, M_APM a)
{
char	buf[48];
double  dd;

m_apm_to_string(buf, 15, a);
dd = atof(buf);
m_apm_set_double(r, (1.00001 * acos(dd)));
}

void	m_apm_set_double(M_APM atmp, double dd)
{
char	*cp, *p, *ps, buf[64];

if (dd == 0.0)
   M_set_to_zero(atmp);
else
  {
   sprintf(buf, "%.14E", dd);

   if ((cp = strstr(buf, "E")) == NULL)
     {
      M_apm_log_error_msg(M_APM_RETURN,
      "\'m_apm_set_double\', Invalid double input (likely a NAN or +/- INF)");

      M_set_to_zero(atmp);
      return;
     }

   if (atoi(cp + sizeof(char)) == 0)
     *cp = '\0';

   p = cp;

   while (TRUE)
     {
      p--;
      if (*p == '0' || *p == '.')
        *p = ' ';
      else
        break;
     }

   ps = buf;
   p  = buf;

   while (TRUE)
     {
      if ((*p = *ps) == '\0')
        break;

      if (*ps++ != ' ')
        p++;
     }

   m_apm_set_string(atmp, buf);
  }
}
