
#include "m_apm_lc.h"

void	m_apm_free_all_mem()
{
M_free_all_add();
M_free_all_div();
M_free_all_exp();

#ifndef NO_FFT_MULTIPLY
M_free_all_fft();
#endif

M_free_all_pow();
M_free_all_rnd();
M_free_all_set();
M_free_all_cnst();
M_free_all_fmul();
M_free_all_stck();
M_free_all_util();
}

void	m_apm_trim_mem_usage()
{
m_apm_free_all_mem();
m_apm_free(m_apm_init());
}
