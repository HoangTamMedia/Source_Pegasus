
#include "m_apm_lc.h"

int	MM_lc_PI_digits = 0;
int	MM_lc_log_digits;
int     MM_cpp_min_precision;

M_APM	MM_Zero          = NULL;
M_APM	MM_One           = NULL;
M_APM	MM_Two           = NULL;
M_APM	MM_Three         = NULL;
M_APM	MM_Four          = NULL;
M_APM	MM_Five          = NULL;
M_APM	MM_Ten           = NULL;
M_APM	MM_0_5           = NULL;
M_APM	MM_E             = NULL;
M_APM	MM_PI            = NULL;
M_APM	MM_HALF_PI       = NULL;
M_APM	MM_2_PI          = NULL;
M_APM	MM_lc_PI         = NULL;
M_APM	MM_lc_HALF_PI    = NULL;
M_APM	MM_lc_2_PI       = NULL;
M_APM	MM_lc_log2       = NULL;
M_APM	MM_lc_log10      = NULL;
M_APM	MM_lc_log10R     = NULL;
M_APM	MM_0_85          = NULL;
M_APM	MM_5x_125R       = NULL;
M_APM	MM_5x_64R        = NULL;
M_APM	MM_5x_256R       = NULL;
M_APM	MM_5x_Eight      = NULL;
M_APM	MM_5x_Sixteen    = NULL;
M_APM	MM_5x_Twenty     = NULL;
M_APM	MM_LOG_E_BASE_10 = NULL;
M_APM	MM_LOG_10_BASE_E = NULL;
M_APM	MM_LOG_2_BASE_E  = NULL;
M_APM	MM_LOG_3_BASE_E  = NULL;

static char MM_cnst_PI[] =
"3.1415926535897932384626433832795028841971693993751058209749445923078\
1640628620899862803482534211706798214808651328230664709384460955";

static char MM_cnst_E[] =
"2.7182818284590452353602874713526624977572470936999595749669676277240\
76630353547594571382178525166427427466391932003059921817413596629";

static char MM_cnst_log_2[] =
"0.6931471805599453094172321214581765680755001343602552541206800094933\
93621969694715605863326996418687542001481020570685733685520235758";

static char MM_cnst_log_3[] =
"1.0986122886681096913952452369225257046474905578227494517346943336374\
9429321860896687361575481373208878797002906595786574236800422593";

static char MM_cnst_log_10[] =
"2.3025850929940456840179914546843642076011014886287729760333279009675\
7260967735248023599720508959829834196778404228624863340952546508";

static char MM_cnst_1_log_10[] =
"0.4342944819032518276511289189166050822943970058036665661144537831658\
64649208870774729224949338431748318706106744766303733641679287159";

char	*m_apm_lib_version(char *v)
{
strcpy(v, MAPM_LIB_VERSION);
return(v);
}

char	*m_apm_lib_short_version(char *v)
{
strcpy(v, MAPM_LIB_SHORT_VERSION);
return(v);
}

void	M_free_all_cnst()
{
if (MM_lc_PI_digits != 0)
  {
   m_apm_free(MM_Zero);
   m_apm_free(MM_One);
   m_apm_free(MM_Two);
   m_apm_free(MM_Three);
   m_apm_free(MM_Four);
   m_apm_free(MM_Five);
   m_apm_free(MM_Ten);
   m_apm_free(MM_0_5);
   m_apm_free(MM_LOG_2_BASE_E);
   m_apm_free(MM_LOG_3_BASE_E);
   m_apm_free(MM_E);
   m_apm_free(MM_PI);
   m_apm_free(MM_HALF_PI);
   m_apm_free(MM_2_PI);
   m_apm_free(MM_lc_PI);
   m_apm_free(MM_lc_HALF_PI);
   m_apm_free(MM_lc_2_PI);
   m_apm_free(MM_lc_log2);
   m_apm_free(MM_lc_log10);
   m_apm_free(MM_lc_log10R);
   m_apm_free(MM_0_85);
   m_apm_free(MM_5x_125R);
   m_apm_free(MM_5x_64R);
   m_apm_free(MM_5x_256R);
   m_apm_free(MM_5x_Eight);
   m_apm_free(MM_5x_Sixteen);
   m_apm_free(MM_5x_Twenty);
   m_apm_free(MM_LOG_E_BASE_10);
   m_apm_free(MM_LOG_10_BASE_E);

   MM_lc_PI_digits = 0;
  }
}

void	M_init_trig_globals()
{
MM_lc_PI_digits      = VALID_DECIMAL_PLACES;
MM_lc_log_digits     = VALID_DECIMAL_PLACES;
MM_cpp_min_precision = 30;

MM_Zero          = m_apm_init();
MM_One           = m_apm_init();
MM_Two           = m_apm_init();
MM_Three         = m_apm_init();
MM_Four          = m_apm_init();
MM_Five          = m_apm_init();
MM_Ten           = m_apm_init();
MM_0_5           = m_apm_init();
MM_LOG_2_BASE_E  = m_apm_init();
MM_LOG_3_BASE_E  = m_apm_init();
MM_E             = m_apm_init();
MM_PI            = m_apm_init();
MM_HALF_PI       = m_apm_init();
MM_2_PI          = m_apm_init();
MM_lc_PI         = m_apm_init();
MM_lc_HALF_PI    = m_apm_init();
MM_lc_2_PI       = m_apm_init();
MM_lc_log2       = m_apm_init();
MM_lc_log10      = m_apm_init();
MM_lc_log10R     = m_apm_init();
MM_0_85          = m_apm_init();
MM_5x_125R       = m_apm_init();
MM_5x_64R        = m_apm_init();
MM_5x_256R       = m_apm_init();
MM_5x_Eight      = m_apm_init();
MM_5x_Sixteen    = m_apm_init();
MM_5x_Twenty     = m_apm_init();
MM_LOG_E_BASE_10 = m_apm_init();
MM_LOG_10_BASE_E = m_apm_init();

m_apm_set_string(MM_One, "1");
m_apm_set_string(MM_Two, "2");
m_apm_set_string(MM_Three, "3");
m_apm_set_string(MM_Four, "4");
m_apm_set_string(MM_Five, "5");
m_apm_set_string(MM_Ten, "10");
m_apm_set_string(MM_0_5, "0.5");
m_apm_set_string(MM_0_85, "0.85");

m_apm_set_string(MM_5x_125R, "8.0E-3");
m_apm_set_string(MM_5x_64R, "1.5625E-2");
m_apm_set_string(MM_5x_256R, "3.90625E-3");
m_apm_set_string(MM_5x_Eight, "8");
m_apm_set_string(MM_5x_Sixteen, "16");
m_apm_set_string(MM_5x_Twenty, "20");

m_apm_set_string(MM_LOG_2_BASE_E, MM_cnst_log_2);
m_apm_set_string(MM_LOG_3_BASE_E, MM_cnst_log_3);
m_apm_set_string(MM_LOG_10_BASE_E, MM_cnst_log_10);
m_apm_set_string(MM_LOG_E_BASE_10, MM_cnst_1_log_10);

m_apm_set_string(MM_lc_log2, MM_cnst_log_2);
m_apm_set_string(MM_lc_log10, MM_cnst_log_10);
m_apm_set_string(MM_lc_log10R, MM_cnst_1_log_10);

m_apm_set_string(MM_E, MM_cnst_E);
m_apm_set_string(MM_PI, MM_cnst_PI);
m_apm_multiply(MM_HALF_PI, MM_PI, MM_0_5);
m_apm_multiply(MM_2_PI, MM_PI, MM_Two);

m_apm_copy(MM_lc_PI, MM_PI);
m_apm_copy(MM_lc_HALF_PI, MM_HALF_PI);
m_apm_copy(MM_lc_2_PI, MM_2_PI);
}

void	m_apm_cpp_precision(int digits)
{
if (MM_lc_PI_digits == 0)
  {
   m_apm_free(m_apm_init());
  }

if (digits >= 2)
  MM_cpp_min_precision = digits;
else
  MM_cpp_min_precision = 2;
}
