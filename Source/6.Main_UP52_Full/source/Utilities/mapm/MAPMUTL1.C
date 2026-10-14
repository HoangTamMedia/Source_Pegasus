
#include "m_apm_lc.h"

void	M_apm_log_error_msg(int fatal, char *message)
{
if (fatal)
  {
   fprintf(stderr, "MAPM Error: %s\n", message);
   exit(100);
  }
else
  {
   fprintf(stderr, "MAPM Warning: %s\n", message);
  }
}
