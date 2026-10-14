
#ifndef M__APMMT__INCLUDED
#define M__APMMT__INCLUDED

#ifdef __cplusplus

#define APM_CONVERT_FROM_C
#endif

#ifdef APM_CONVERT_FROM_C
extern "C" {
#endif

typedef unsigned char UCHAR;

typedef struct  {
	UCHAR	*m_apm_data;
	long	m_apm_id;
	int     m_apm_refcount;
	int	m_apm_malloclength;
	int	m_apm_datalength;
	int	m_apm_exponent;
	int	m_apm_sign;
} M_APM_struct;

typedef M_APM_struct *M_APM;

#define MAPM_LIB_VERSION \
    "MAPM Library Version 4.9.5  Copyright (C) 1999-2007, Michael C. Ring"
#define MAPM_LIB_SHORT_VERSION "4.9.5"

extern	M_APM	MM_Zero;
extern	M_APM	MM_One;
extern	M_APM	MM_Two;
extern	M_APM	MM_Three;
extern	M_APM	MM_Four;
extern	M_APM	MM_Five;
extern	M_APM	MM_Ten;

extern	M_APM	MM_PI;
extern	M_APM	MM_HALF_PI;
extern	M_APM	MM_2_PI;
extern	M_APM	MM_E;

extern	M_APM	MM_LOG_E_BASE_10;
extern	M_APM	MM_LOG_10_BASE_E;
extern	M_APM	MM_LOG_2_BASE_E;
extern	M_APM	MM_LOG_3_BASE_E;

extern	void	m_apm_enter(void);
extern	void	m_apm_leave(void);
extern	void	m_apm_init_semaphore(void);
extern	void	m_apm_free_semaphore(void);

extern	M_APM	m_apm_init_mt(void);
extern	void	m_apm_free_mt(M_APM);
extern	void	m_apm_free_all_mem_mt(void);
extern	void	m_apm_trim_mem_usage_mt(void);
extern	char	*m_apm_lib_version_mt(char *);
extern	char	*m_apm_lib_short_version_mt(char *);

extern	void	m_apm_set_string_mt(M_APM, char *);
extern	void	m_apm_set_double_mt(M_APM, double);
extern	void	m_apm_set_long_mt(M_APM, long);

extern	void	m_apm_to_string_mt(char *, int, M_APM);
extern  void	m_apm_to_fixpt_string_mt(char *, int, M_APM);
extern  void	m_apm_to_fixpt_stringex_mt(char *, int, M_APM, char,
					   char, int);

extern  char	*m_apm_to_fixpt_stringexp_mt(int, M_APM, char, char, int);
extern  void    m_apm_to_integer_string_mt(char *, M_APM);

extern	void	m_apm_absolute_value_mt(M_APM, M_APM);
extern	void	m_apm_negate_mt(M_APM, M_APM);
extern	void	m_apm_copy_mt(M_APM, M_APM);
extern	void	m_apm_round_mt(M_APM, int, M_APM);
extern	int	m_apm_compare_mt(M_APM, M_APM);
extern	int	m_apm_sign_mt(M_APM);
extern	int	m_apm_exponent_mt(M_APM);
extern	int	m_apm_significant_digits_mt(M_APM);
extern	int	m_apm_is_integer_mt(M_APM);
extern	int	m_apm_is_even_mt(M_APM);
extern	int	m_apm_is_odd_mt(M_APM);

extern	void	m_apm_gcd_mt(M_APM, M_APM, M_APM);
extern	void	m_apm_lcm_mt(M_APM, M_APM, M_APM);

extern	void	m_apm_add_mt(M_APM, M_APM, M_APM);
extern	void	m_apm_subtract_mt(M_APM, M_APM, M_APM);
extern	void	m_apm_multiply_mt(M_APM, M_APM, M_APM);
extern	void	m_apm_divide_mt(M_APM, int, M_APM, M_APM);
extern	void	m_apm_integer_divide_mt(M_APM, M_APM, M_APM);
extern	void	m_apm_integer_div_rem_mt(M_APM, M_APM, M_APM, M_APM);
extern	void	m_apm_reciprocal_mt(M_APM, int, M_APM);
extern	void	m_apm_factorial_mt(M_APM, M_APM);
extern	void	m_apm_floor_mt(M_APM, M_APM);
extern	void	m_apm_ceil_mt(M_APM, M_APM);
extern	void	m_apm_get_random_mt(M_APM);
extern	void	m_apm_set_random_seed_mt(char *);

extern	void	m_apm_sqrt_mt(M_APM, int, M_APM);
extern	void	m_apm_cbrt_mt(M_APM, int, M_APM);
extern	void	m_apm_log_mt(M_APM, int, M_APM);
extern	void	m_apm_log10_mt(M_APM, int, M_APM);
extern	void	m_apm_exp_mt(M_APM, int, M_APM);
extern	void	m_apm_pow_mt(M_APM, int, M_APM, M_APM);
extern  void	m_apm_integer_pow_mt(M_APM, int, M_APM, int);
extern  void	m_apm_integer_pow_nr_mt(M_APM, M_APM, int);

extern	void	m_apm_sin_cos_mt(M_APM, M_APM, int, M_APM);
extern	void	m_apm_sin_mt(M_APM, int, M_APM);
extern	void	m_apm_cos_mt(M_APM, int, M_APM);
extern	void	m_apm_tan_mt(M_APM, int, M_APM);
extern	void	m_apm_arcsin_mt(M_APM, int, M_APM);
extern	void	m_apm_arccos_mt(M_APM, int, M_APM);
extern	void	m_apm_arctan_mt(M_APM, int, M_APM);
extern	void	m_apm_arctan2_mt(M_APM, int, M_APM, M_APM);

extern  void    m_apm_sinh_mt(M_APM, int, M_APM);
extern  void    m_apm_cosh_mt(M_APM, int, M_APM);
extern  void    m_apm_tanh_mt(M_APM, int, M_APM);
extern  void    m_apm_arcsinh_mt(M_APM, int, M_APM);
extern  void    m_apm_arccosh_mt(M_APM, int, M_APM);
extern  void    m_apm_arctanh_mt(M_APM, int, M_APM);

extern  void    m_apm_cpp_precision_mt(int);

#define m_apm_asin_mt m_apm_arcsin_mt
#define m_apm_acos_mt m_apm_arccos_mt
#define m_apm_atan_mt m_apm_arctan_mt
#define m_apm_atan2_mt m_apm_arctan2_mt

#define m_apm_asinh_mt m_apm_arcsinh_mt
#define m_apm_acosh_mt m_apm_arccosh_mt
#define m_apm_atanh_mt m_apm_arctanh_mt

#ifdef APM_CONVERT_FROM_C
}
#endif

#ifdef __cplusplus

extern
#ifdef APM_CONVERT_FROM_C
"C"
#endif
int MM_cpp_min_precision;

class MAPMMT {
protected:

public:
	M_APM myVal;
protected:
	void create(void) {myVal=makeNew();}
	void destroy(void) {unref(myVal);myVal=NULL;}
	void copyFrom(M_APM Nval)
	{
		 M_APM oldVal=myVal;
		 myVal=Nval;
		 ref(myVal);
		 unref(oldVal);
	}
	static M_APM makeNew(void)
	{
		M_APM val=m_apm_init_mt();

		return val;
	}
	static void ref(M_APM val)
	{
		val->m_apm_refcount++;
	}
	static void unref(M_APM val)
	{
		val->m_apm_refcount--;
		if (val->m_apm_refcount==0)
			m_apm_free_mt(val);
	}

	M_APM val(void)
	{
		if (myVal->m_apm_refcount==1)

			return myVal;

		M_APM oldVal=myVal;
		myVal=makeNew();
		m_apm_copy_mt(myVal,oldVal);
		unref(oldVal);
		return myVal;
	}

	M_APM cval(void) const
	{
		return (M_APM)myVal;
	}

	int myDigits(void) const
	{
		int maxd=m_apm_significant_digits_mt(cval());
		if (maxd<MM_cpp_min_precision) maxd=MM_cpp_min_precision;
		return maxd;
	}

	int digits(const MAPMMT &otherVal) const
	{
		int maxd=myDigits();
		int his=m_apm_significant_digits_mt(otherVal.cval());
		if (maxd<his) maxd=his;
		return maxd;
	}
public:

	MAPMMT(void)
		{m_apm_enter();create();m_apm_leave();}
	MAPMMT(const MAPMMT &m)
		{m_apm_enter();myVal=(M_APM)m.cval();ref(myVal);m_apm_leave();}
	MAPMMT(M_APM m)
		{m_apm_enter();myVal=(M_APM)m;ref(myVal);m_apm_leave();}
	MAPMMT(const char *s)
		{m_apm_enter();create();m_apm_set_string_mt(val(),(char *)s);m_apm_leave();}
	MAPMMT(double d)
		{m_apm_enter();create();m_apm_set_double_mt(val(),d);m_apm_leave();}
	MAPMMT(int l)
		{m_apm_enter();create();m_apm_set_long_mt(val(),l);m_apm_leave();}
	MAPMMT(long l)
		{m_apm_enter();create();m_apm_set_long_mt(val(),l);m_apm_leave();}

	~MAPMMT() {m_apm_enter();destroy();m_apm_leave();}

	void toString(char *dest,int decimalPlaces) const
		{m_apm_to_string_mt(dest,decimalPlaces,cval());}
	void toFixPtString(char *dest,int decimalPlaces) const
		{m_apm_to_fixpt_string_mt(dest,decimalPlaces,cval());}
	void toFixPtStringEx(char *dest,int dp,char a,char b,int c) const
		{m_apm_to_fixpt_stringex_mt(dest,dp,cval(),a,b,c);}
	char *toFixPtStringExp(int dp,char a,char b,int c) const
		{return(m_apm_to_fixpt_stringexp_mt(dp,cval(),a,b,c));}
	void toIntegerString(char *dest) const
		{m_apm_to_integer_string_mt(dest,cval());}

	MAPMMT &operator=(const MAPMMT &m)
		{m_apm_enter();copyFrom((M_APM)m.cval());m_apm_leave();return *this;}
	MAPMMT &operator=(const char *s)
		{m_apm_set_string_mt(val(),(char *)s);return *this;}
	MAPMMT &operator=(double d)
		{m_apm_set_double_mt(val(),d);return *this;}
	MAPMMT &operator=(int l)
		{m_apm_set_long_mt(val(),l);return *this;}
	MAPMMT &operator=(long l)
		{m_apm_set_long_mt(val(),l);return *this;}
	MAPMMT operator++()
		{m_apm_enter();*this = *this+MM_One;m_apm_leave();return *this;}
	MAPMMT operator--()
		{m_apm_enter();*this = *this-MM_One;m_apm_leave();return *this;}
	const MAPMMT operator++(int)
	{
		m_apm_enter();
		MAPMMT old = *this;
		++(*this);
		m_apm_leave();
		return old;
	}
	const MAPMMT operator--(int)
	{
		m_apm_enter();
		MAPMMT old = *this;
		--(*this);
		m_apm_leave();
		return old;
	}

	int operator==(const MAPMMT &m) const
	 {return m_apm_compare_mt(cval(),m.cval())==0;}
	int operator!=(const MAPMMT &m) const
	 {return m_apm_compare_mt(cval(),m.cval())!=0;}
	int operator<(const MAPMMT &m) const
	 {return m_apm_compare_mt(cval(),m.cval())<0;}
	int operator<=(const MAPMMT &m) const
	 {return m_apm_compare_mt(cval(),m.cval())<=0;}
	int operator>(const MAPMMT &m) const
	 {return m_apm_compare_mt(cval(),m.cval())>0;}
	int operator>=(const MAPMMT &m) const
	 {return m_apm_compare_mt(cval(),m.cval())>=0;}

	friend MAPMMT operator+(const MAPMMT &a,const MAPMMT &b)
	 {MAPMMT ret;m_apm_add_mt(ret.val(),a.cval(),b.cval());return ret;}
	friend MAPMMT operator-(const MAPMMT &a,const MAPMMT &b)
	 {MAPMMT ret;m_apm_subtract_mt(ret.val(),a.cval(),b.cval());return ret;}
	friend MAPMMT operator*(const MAPMMT &a,const MAPMMT &b)
	 {MAPMMT ret;m_apm_multiply_mt(ret.val(),a.cval(),b.cval());return ret;}
	friend MAPMMT operator%(const MAPMMT &a,const MAPMMT &b)
	 {MAPMMT quot,ret;m_apm_integer_div_rem_mt(quot.val(),ret.val(),
	 	a.cval(),b.cval());return ret;}

	friend MAPMMT operator/(const MAPMMT &a,const MAPMMT &b)
		{return a.divide(b,a.digits(b));}

	MAPMMT divide(const MAPMMT &m,int toDigits) const
        	{MAPMMT ret;m_apm_divide_mt(ret.val(),toDigits,cval(), m.cval());return ret;}
	MAPMMT divide(const MAPMMT &m) const {return divide(m,digits(m));}

	MAPMMT &operator+=(const MAPMMT &m) {*this = *this+m;return *this;}
	MAPMMT &operator-=(const MAPMMT &m) {*this = *this-m;return *this;}
	MAPMMT &operator*=(const MAPMMT &m) {*this = *this*m;return *this;}
	MAPMMT &operator/=(const MAPMMT &m) {*this = *this/m;return *this;}
	MAPMMT &operator%=(const MAPMMT &m) {*this = *this%m;return *this;}

	int sign(void) const
		{return m_apm_sign_mt(cval());}
	int exponent(void) const
		{return m_apm_exponent_mt(cval());}
	int significant_digits(void) const
		{return m_apm_significant_digits_mt(cval());}
	int is_integer(void) const
		{return m_apm_is_integer_mt(cval());}
	int is_even(void) const
		{return m_apm_is_even_mt(cval());}
	int is_odd(void) const
		{return m_apm_is_odd_mt(cval());}

	MAPMMT abs(void) const
		{MAPMMT ret;m_apm_absolute_value_mt(ret.val(),cval());return ret;}
	MAPMMT neg(void) const
		{MAPMMT ret;m_apm_negate_mt(ret.val(),cval());return ret;}
	MAPMMT round(int toDigits) const
		{MAPMMT ret;m_apm_round_mt(ret.val(),toDigits,cval());return ret;}
	MAPMMT operator-(void) const {return neg();}

#define MAPMMT_1aryFunc(func) \
	MAPMMT func(int toDigits) const\
		{MAPMMT ret;m_apm_##func##_mt(ret.val(),toDigits,cval());return ret;}\
	MAPMMT func(void) const {return func(myDigits());}

	MAPMMT_1aryFunc(sqrt)
	MAPMMT_1aryFunc(cbrt)
	MAPMMT_1aryFunc(log)
	MAPMMT_1aryFunc(exp)
	MAPMMT_1aryFunc(log10)
	MAPMMT_1aryFunc(sin)
	MAPMMT_1aryFunc(asin)
	MAPMMT_1aryFunc(cos)
	MAPMMT_1aryFunc(acos)
	MAPMMT_1aryFunc(tan)
	MAPMMT_1aryFunc(atan)
	MAPMMT_1aryFunc(sinh)
	MAPMMT_1aryFunc(asinh)
	MAPMMT_1aryFunc(cosh)
	MAPMMT_1aryFunc(acosh)
	MAPMMT_1aryFunc(tanh)
	MAPMMT_1aryFunc(atanh)
#undef MAPMMT_1aryFunc

	void sincos(MAPMMT &sinR,MAPMMT &cosR,int toDigits)
		{m_apm_sin_cos_mt(sinR.val(),cosR.val(),toDigits,cval());}
	void sincos(MAPMMT &sinR,MAPMMT &cosR)
		{sincos(sinR,cosR,myDigits());}
	MAPMMT pow(const MAPMMT &m,int toDigits) const
		{MAPMMT ret;m_apm_pow_mt(ret.val(),toDigits,cval(),
					  m.cval());return ret;}
	MAPMMT pow(const MAPMMT &m) const {return pow(m,digits(m));}
	MAPMMT atan2(const MAPMMT &x,int toDigits) const
		{MAPMMT ret;m_apm_arctan2_mt(ret.val(),toDigits,cval(),
					    x.cval());return ret;}
	MAPMMT atan2(const MAPMMT &x) const
		{return atan2(x,digits(x));}

	MAPMMT gcd(const MAPMMT &m) const
		{MAPMMT ret;m_apm_gcd_mt(ret.val(),cval(),m.cval());return ret;}

	MAPMMT lcm(const MAPMMT &m) const
		{MAPMMT ret;m_apm_lcm_mt(ret.val(),cval(),m.cval());return ret;}

	static MAPMMT random(void)
		{MAPMMT ret;m_apm_get_random_mt(ret.val());return ret;}

	MAPMMT floor(void) const
		{MAPMMT ret;m_apm_floor_mt(ret.val(),cval());return ret;}
	MAPMMT ceil(void) const
		{MAPMMT ret;m_apm_ceil_mt(ret.val(),cval());return ret;}

	MAPMMT factorial(void) const
		{MAPMMT ret;m_apm_factorial_mt(ret.val(),cval());return ret;}
	MAPMMT ipow_nr(int p) const
		{MAPMMT ret;m_apm_integer_pow_nr_mt(ret.val(),
				cval(),p);return ret;}
	MAPMMT ipow(int p,int toDigits) const
		{MAPMMT ret;m_apm_integer_pow_mt(ret.val(),
				toDigits,cval(),p);return ret;}
	MAPMMT ipow(int p) const
		{return ipow(p,myDigits());}

	MAPMMT integer_divide(const MAPMMT &denom) const
		{MAPMMT ret;m_apm_integer_divide_mt(ret.val(),cval(),
		                       denom.cval());return ret;}
	void integer_div_rem(const MAPMMT &denom,MAPMMT &quot,MAPMMT &rem) const
		{m_apm_integer_div_rem_mt(quot.val(),rem.val(),cval(),
					             denom.cval());}
	MAPMMT div(const MAPMMT &denom) const {return integer_divide(denom);}
	MAPMMT rem(const MAPMMT &denom) const {MAPMMT ret,ignored;
		integer_div_rem(denom,ignored,ret);return ret;}
};

inline MAPMMT fabs(const MAPMMT &m) {return m.abs();}
inline MAPMMT factorial(const MAPMMT &m) {return m.factorial();}
inline MAPMMT floor(const MAPMMT &m) {return m.floor();}
inline MAPMMT ceil(const MAPMMT &m) {return m.ceil();}
inline MAPMMT get_random(void) {return MAPMMT::random();}

#define MAPMMT_1aryFunc(func) \
	inline MAPMMT func(const MAPMMT &m) {return m.func();} \
	inline MAPMMT func(const MAPMMT &m,int toDigits) {return m.func(toDigits);}

	MAPMMT_1aryFunc(sqrt)
	MAPMMT_1aryFunc(cbrt)
	MAPMMT_1aryFunc(log)
	MAPMMT_1aryFunc(exp)
	MAPMMT_1aryFunc(log10)
	MAPMMT_1aryFunc(sin)
	MAPMMT_1aryFunc(asin)
	MAPMMT_1aryFunc(cos)
	MAPMMT_1aryFunc(acos)
	MAPMMT_1aryFunc(tan)
	MAPMMT_1aryFunc(atan)
	MAPMMT_1aryFunc(sinh)
	MAPMMT_1aryFunc(asinh)
	MAPMMT_1aryFunc(cosh)
	MAPMMT_1aryFunc(acosh)
	MAPMMT_1aryFunc(tanh)
	MAPMMT_1aryFunc(atanh)
#undef MAPMMT_1aryFunc

inline MAPMMT pow(const MAPMMT &x,const MAPMMT &y,int toDigits)
		{return x.pow(y,toDigits);}
inline MAPMMT pow(const MAPMMT &x,const MAPMMT &y)
		{return x.pow(y);}
inline MAPMMT atan2(const MAPMMT &y,const MAPMMT &x,int toDigits)
		{return y.atan2(x,toDigits);}
inline MAPMMT atan2(const MAPMMT &y,const MAPMMT &x)
		{return y.atan2(x);}
inline MAPMMT gcd(const MAPMMT &u,const MAPMMT &v)
		{return u.gcd(v);}
inline MAPMMT lcm(const MAPMMT &u,const MAPMMT &v)
		{return u.lcm(v);}
#endif
#endif
