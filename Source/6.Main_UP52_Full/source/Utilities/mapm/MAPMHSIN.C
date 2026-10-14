
#include "m_apm_lc.h"

void	m_apm_sinh(M_APM rr, int places, M_APM aa)
{
M_APM	tmp1, tmp2, tmp3;
int     local_precision;

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();
tmp3 = M_get_stack_var();

local_precision = places + 4;

m_apm_exp(tmp1, local_precision, aa);
m_apm_reciprocal(tmp2, local_precision, tmp1);
m_apm_subtract(tmp3, tmp1, tmp2);
m_apm_multiply(tmp1, tmp3, MM_0_5);
m_apm_round(rr, places, tmp1);

M_restore_stack(3);
}

void	m_apm_cosh(M_APM rr, int places, M_APM aa)
{
M_APM	tmp1, tmp2, tmp3;
int     local_precision;

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();
tmp3 = M_get_stack_var();

local_precision = places + 4;

m_apm_exp(tmp1, local_precision, aa);
m_apm_reciprocal(tmp2, local_precision, tmp1);
m_apm_add(tmp3, tmp1, tmp2);
m_apm_multiply(tmp1, tmp3, MM_0_5);
m_apm_round(rr, places, tmp1);

M_restore_stack(3);
}

void	m_apm_tanh(M_APM rr, int places, M_APM aa)
{
M_APM	tmp1, tmp2, tmp3, tmp4;
int     local_precision;

tmp1 = M_get_stack_var();
tmp2 = M_get_stack_var();
tmp3 = M_get_stack_var();
tmp4 = M_get_stack_var();

local_precision = places + 4;

m_apm_exp(tmp1, local_precision, aa);
m_apm_reciprocal(tmp2, local_precision, tmp1);
m_apm_subtract(tmp3, tmp1, tmp2);
m_apm_add(tmp4, tmp1, tmp2);
m_apm_divide(tmp1, local_precision, tmp3, tmp4);
m_apm_round(rr, places, tmp1);

M_restore_stack(4);
}
