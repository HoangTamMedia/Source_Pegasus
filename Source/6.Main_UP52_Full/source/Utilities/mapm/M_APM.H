
#ifndef M__APM__INCLUDED
#define M__APM__INCLUDED

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

extern	M_APM	m_apm_init(void);
extern	void	m_apm_free(M_APM);
extern	void	m_apm_free_all_mem(void);
extern	void	m_apm_trim_mem_usage(void);
extern	char	*m_apm_lib_version(char *);
extern	char	*m_apm_lib_short_version(char *);

extern	void	m_apm_set_string(M_APM, char *);
extern	void	m_apm_set_double(M_APM, double);
extern	void	m_apm_set_long(M_APM, long);

extern	void	m_apm_to_string(char *, int, M_APM);
extern  void	m_apm_to_fixpt_string(char *, int, M_APM);
extern  void	m_apm_to_fixpt_stringex(char *, int, M_APM, char, char, int);
extern  char	*m_apm_to_fixpt_stringexp(int, M_APM, char, char, int);
extern  void    m_apm_to_integer_string(char *, M_APM);

extern	void	m_apm_absolute_value(M_APM, M_APM);
extern	void	m_apm_negate(M_APM, M_APM);
extern	void	m_apm_copy(M_APM, M_APM);
extern	void	m_apm_round(M_APM, int, M_APM);
extern	int	m_apm_compare(M_APM, M_APM);
extern	int	m_apm_sign(M_APM);
extern	int	m_apm_exponent(M_APM);
extern	int	m_apm_significant_digits(M_APM);
extern	int	m_apm_is_integer(M_APM);
extern	int	m_apm_is_even(M_APM);
extern	int	m_apm_is_odd(M_APM);

extern	void	m_apm_gcd(M_APM, M_APM, M_APM);
extern	void	m_apm_lcm(M_APM, M_APM, M_APM);

extern	void	m_apm_add(M_APM, M_APM, M_APM);
extern	void	m_apm_subtract(M_APM, M_APM, M_APM);
extern	void	m_apm_multiply(M_APM, M_APM, M_APM);
extern	void	m_apm_divide(M_APM, int, M_APM, M_APM);
extern	void	m_apm_integer_divide(M_APM, M_APM, M_APM);
extern	void	m_apm_integer_div_rem(M_APM, M_APM, M_APM, M_APM);
extern	void	m_apm_reciprocal(M_APM, int, M_APM);
extern	void	m_apm_factorial(M_APM, M_APM);
extern	void	m_apm_floor(M_APM, M_APM);
extern	void	m_apm_ceil(M_APM, M_APM);
extern	void	m_apm_get_random(M_APM);
extern	void	m_apm_set_random_seed(char *);

extern	void	m_apm_sqrt(M_APM, int, M_APM);
extern	void	m_apm_cbrt(M_APM, int, M_APM);
extern	void	m_apm_log(M_APM, int, M_APM);
extern	void	m_apm_log10(M_APM, int, M_APM);
extern	void	m_apm_exp(M_APM, int, M_APM);
extern	void	m_apm_pow(M_APM, int, M_APM, M_APM);
extern  void	m_apm_integer_pow(M_APM, int, M_APM, int);
extern  void	m_apm_integer_pow_nr(M_APM, M_APM, int);

extern	void	m_apm_sin_cos(M_APM, M_APM, int, M_APM);
extern	void	m_apm_sin(M_APM, int, M_APM);
extern	void	m_apm_cos(M_APM, int, M_APM);
extern	void	m_apm_tan(M_APM, int, M_APM);
extern	void	m_apm_arcsin(M_APM, int, M_APM);
extern	void	m_apm_arccos(M_APM, int, M_APM);
extern	void	m_apm_arctan(M_APM, int, M_APM);
extern	void	m_apm_arctan2(M_APM, int, M_APM, M_APM);

extern  void    m_apm_sinh(M_APM, int, M_APM);
extern  void    m_apm_cosh(M_APM, int, M_APM);
extern  void    m_apm_tanh(M_APM, int, M_APM);
extern  void    m_apm_arcsinh(M_APM, int, M_APM);
extern  void    m_apm_arccosh(M_APM, int, M_APM);
extern  void    m_apm_arctanh(M_APM, int, M_APM);

extern  void    m_apm_cpp_precision(int);

#define m_apm_asin m_apm_arcsin
#define m_apm_acos m_apm_arccos
#define m_apm_atan m_apm_arctan
#define m_apm_atan2 m_apm_arctan2

#define m_apm_asinh m_apm_arcsinh
#define m_apm_acosh m_apm_arccosh
#define m_apm_atanh m_apm_arctanh

#ifdef APM_CONVERT_FROM_C
}
#endif

#ifdef __cplusplus

extern
#ifdef APM_CONVERT_FROM_C
"C"
#endif
int MM_cpp_min_precision;

class MAPM {
protected:

	M_APM myVal;
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
		M_APM val=m_apm_init();

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
			m_apm_free(val);
	}

	M_APM val(void)
	{
		if (myVal->m_apm_refcount==1)

			return myVal;

		M_APM oldVal=myVal;
		myVal=makeNew();
		m_apm_copy(myVal,oldVal);
		unref(oldVal);
		return myVal;
	}

	M_APM cval(void) const
	{
		return (M_APM)myVal;
	}

	int myDigits(void) const
	{
		int maxd=m_apm_significant_digits(cval());
		if (maxd<MM_cpp_min_precision) maxd=MM_cpp_min_precision;
		return maxd;
	}

	int digits(const MAPM &otherVal) const
	{
		int maxd=myDigits();
		int his=m_apm_significant_digits(otherVal.cval());
		if (maxd<his) maxd=his;
		return maxd;
	}
public:

	MAPM(void)
		{create();}
	MAPM(const MAPM &m)
		{myVal=(M_APM)m.cval();ref(myVal);}
	MAPM(M_APM m)
		{myVal=(M_APM)m;ref(myVal);}
	MAPM(const char *s)
		{create();m_apm_set_string(val(),(char *)s);}
	MAPM(double d)
		{create();m_apm_set_double(val(),d);}
	MAPM(int l)
		{create();m_apm_set_long(val(),l);}
	MAPM(long l)
		{create();m_apm_set_long(val(),l);}

	~MAPM() {destroy();}

	void toString(char *dest,int decimalPlaces) const
		{m_apm_to_string(dest,decimalPlaces,cval());}
	void toFixPtString(char *dest,int decimalPlaces) const
		{m_apm_to_fixpt_string(dest,decimalPlaces,cval());}
	void toFixPtStringEx(char *dest,int dp,char a,char b,int c) const
		{m_apm_to_fixpt_stringex(dest,dp,cval(),a,b,c);}
	char *toFixPtStringExp(int dp,char a,char b,int c) const
		{return(m_apm_to_fixpt_stringexp(dp,cval(),a,b,c));}
	void toIntegerString(char *dest) const
		{m_apm_to_integer_string(dest,cval());}

	MAPM &operator=(const MAPM &m)
		{copyFrom((M_APM)m.cval());return *this;}
	MAPM &operator=(const char *s)
		{m_apm_set_string(val(),(char *)s);return *this;}
	MAPM &operator=(double d)
		{m_apm_set_double(val(),d);return *this;}
	MAPM &operator=(int l)
		{m_apm_set_long(val(),l);return *this;}
	MAPM &operator=(long l)
		{m_apm_set_long(val(),l);return *this;}
	MAPM operator++()
		{return *this = *this+MM_One;}
	MAPM operator--()
		{return *this = *this-MM_One;}
	const MAPM operator++(int)
	{
		MAPM old = *this;
		++(*this);
		return old;
	}
	const MAPM operator--(int)
	{
		MAPM old = *this;
		--(*this);
		return old;
	}

	int operator==(const MAPM &m) const
	 {return m_apm_compare(cval(),m.cval())==0;}
	int operator!=(const MAPM &m) const
	 {return m_apm_compare(cval(),m.cval())!=0;}
	int operator<(const MAPM &m) const
	 {return m_apm_compare(cval(),m.cval())<0;}
	int operator<=(const MAPM &m) const
	 {return m_apm_compare(cval(),m.cval())<=0;}
	int operator>(const MAPM &m) const
	 {return m_apm_compare(cval(),m.cval())>0;}
	int operator>=(const MAPM &m) const
	 {return m_apm_compare(cval(),m.cval())>=0;}

	friend MAPM operator+(const MAPM &a,const MAPM &b)
	 {MAPM ret;m_apm_add(ret.val(),a.cval(),b.cval());return ret;}
	friend MAPM operator-(const MAPM &a,const MAPM &b)
	 {MAPM ret;m_apm_subtract(ret.val(),a.cval(),b.cval());return ret;}
	friend MAPM operator*(const MAPM &a,const MAPM &b)
	 {MAPM ret;m_apm_multiply(ret.val(),a.cval(),b.cval());return ret;}
	friend MAPM operator%(const MAPM &a,const MAPM &b)
	 {MAPM quot,ret;m_apm_integer_div_rem(quot.val(),ret.val(),
		a.cval(),b.cval());return ret;}

	friend MAPM operator/(const MAPM &a,const MAPM &b)
		{return a.divide(b,a.digits(b));}

	MAPM divide(const MAPM &m,int toDigits) const
        	{MAPM ret;m_apm_divide(ret.val(),toDigits,cval(),
					        m.cval());return ret;}
	MAPM divide(const MAPM &m) const {return divide(m,digits(m));}

	MAPM &operator+=(const MAPM &m) {*this = *this+m;return *this;}
	MAPM &operator-=(const MAPM &m) {*this = *this-m;return *this;}
	MAPM &operator*=(const MAPM &m) {*this = *this*m;return *this;}
	MAPM &operator/=(const MAPM &m) {*this = *this/m;return *this;}
	MAPM &operator%=(const MAPM &m) {*this = *this%m;return *this;}

	int sign(void) const
		{return m_apm_sign(cval());}
	int exponent(void) const
		{return m_apm_exponent(cval());}
	int significant_digits(void) const
		{return m_apm_significant_digits(cval());}
	int is_integer(void) const
		{return m_apm_is_integer(cval());}
	int is_even(void) const
		{return m_apm_is_even(cval());}
	int is_odd(void) const
		{return m_apm_is_odd(cval());}

	MAPM abs(void) const
		{MAPM ret;m_apm_absolute_value(ret.val(),cval());return ret;}
	MAPM neg(void) const
		{MAPM ret;m_apm_negate(ret.val(),cval());return ret;}
	MAPM round(int toDigits) const
		{MAPM ret;m_apm_round(ret.val(),toDigits,cval());return ret;}
	MAPM operator-(void) const {return neg();}

#define MAPM_1aryFunc(func) \
	MAPM func(int toDigits) const\
		{MAPM ret;m_apm_##func(ret.val(),toDigits,cval());return ret;}\
	MAPM func(void) const {return func(myDigits());}

	MAPM_1aryFunc(sqrt)
	MAPM_1aryFunc(cbrt)
	MAPM_1aryFunc(log)
	MAPM_1aryFunc(exp)
	MAPM_1aryFunc(log10)
	MAPM_1aryFunc(sin)
	MAPM_1aryFunc(asin)
	MAPM_1aryFunc(cos)
	MAPM_1aryFunc(acos)
	MAPM_1aryFunc(tan)
	MAPM_1aryFunc(atan)
	MAPM_1aryFunc(sinh)
	MAPM_1aryFunc(asinh)
	MAPM_1aryFunc(cosh)
	MAPM_1aryFunc(acosh)
	MAPM_1aryFunc(tanh)
	MAPM_1aryFunc(atanh)
#undef MAPM_1aryFunc

	void sincos(MAPM &sinR,MAPM &cosR,int toDigits)
		{m_apm_sin_cos(sinR.val(),cosR.val(),toDigits,cval());}
	void sincos(MAPM &sinR,MAPM &cosR)
		{sincos(sinR,cosR,myDigits());}
	MAPM pow(const MAPM &m,int toDigits) const
		{MAPM ret;m_apm_pow(ret.val(),toDigits,cval(),
					  m.cval());return ret;}
	MAPM pow(const MAPM &m) const {return pow(m,digits(m));}
	MAPM atan2(const MAPM &x,int toDigits) const
		{MAPM ret;m_apm_arctan2(ret.val(),toDigits,cval(),
					    x.cval());return ret;}
	MAPM atan2(const MAPM &x) const
		{return atan2(x,digits(x));}

	MAPM gcd(const MAPM &m) const
		{MAPM ret;m_apm_gcd(ret.val(),cval(),m.cval());return ret;}

	MAPM lcm(const MAPM &m) const
		{MAPM ret;m_apm_lcm(ret.val(),cval(),m.cval());return ret;}

	static MAPM random(void)
		{MAPM ret;m_apm_get_random(ret.val());return ret;}

	MAPM floor(void) const
		{MAPM ret;m_apm_floor(ret.val(),cval());return ret;}
	MAPM ceil(void) const
		{MAPM ret;m_apm_ceil(ret.val(),cval());return ret;}

	MAPM factorial(void) const
		{MAPM ret;m_apm_factorial(ret.val(),cval());return ret;}
	MAPM ipow_nr(int p) const
		{MAPM ret;m_apm_integer_pow_nr(ret.val(),
				cval(),p);return ret;}
	MAPM ipow(int p,int toDigits) const
		{MAPM ret;m_apm_integer_pow(ret.val(),
				toDigits,cval(),p);return ret;}
	MAPM ipow(int p) const
		{return ipow(p,myDigits());}
	MAPM integer_divide(const MAPM &denom) const
		{MAPM ret;m_apm_integer_divide(ret.val(),cval(),
		                       denom.cval());return ret;}
	void integer_div_rem(const MAPM &denom,MAPM &quot,MAPM &rem) const
		{m_apm_integer_div_rem(quot.val(),rem.val(),cval(),
					             denom.cval());}
	MAPM div(const MAPM &denom) const {return integer_divide(denom);}
	MAPM rem(const MAPM &denom) const {MAPM ret,ignored;
		integer_div_rem(denom,ignored,ret);return ret;}
};

inline MAPM fabs(const MAPM &m) {return m.abs();}
inline MAPM factorial(const MAPM &m) {return m.factorial();}
inline MAPM floor(const MAPM &m) {return m.floor();}
inline MAPM ceil(const MAPM &m) {return m.ceil();}
inline MAPM get_random(void) {return MAPM::random();}

#define MAPM_1aryFunc(func) \
	inline MAPM func(const MAPM &m) {return m.func();} \
	inline MAPM func(const MAPM &m,int toDigits) {return m.func(toDigits);}

	MAPM_1aryFunc(sqrt)
	MAPM_1aryFunc(cbrt)
	MAPM_1aryFunc(log)
	MAPM_1aryFunc(exp)
	MAPM_1aryFunc(log10)
	MAPM_1aryFunc(sin)
	MAPM_1aryFunc(asin)
	MAPM_1aryFunc(cos)
	MAPM_1aryFunc(acos)
	MAPM_1aryFunc(tan)
	MAPM_1aryFunc(atan)
	MAPM_1aryFunc(sinh)
	MAPM_1aryFunc(asinh)
	MAPM_1aryFunc(cosh)
	MAPM_1aryFunc(acosh)
	MAPM_1aryFunc(tanh)
	MAPM_1aryFunc(atanh)
#undef MAPM_1aryFunc

inline MAPM pow(const MAPM &x,const MAPM &y,int toDigits)
		{return x.pow(y,toDigits);}
inline MAPM pow(const MAPM &x,const MAPM &y)
		{return x.pow(y);}
inline MAPM atan2(const MAPM &y,const MAPM &x,int toDigits)
		{return y.atan2(x,toDigits);}
inline MAPM atan2(const MAPM &y,const MAPM &x)
		{return y.atan2(x);}
inline MAPM gcd(const MAPM &u,const MAPM &v)
		{return u.gcd(v);}
inline MAPM lcm(const MAPM &u,const MAPM &v)
		{return u.lcm(v);}
#endif
#endif
