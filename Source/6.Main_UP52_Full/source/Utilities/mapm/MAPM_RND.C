
#include "m_apm_lc.h"

#ifndef _HAVE_NI_LABWIN_CVI_
#ifdef MSDOS
#include <time.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
extern  void	M_get_microsec(unsigned long *, long *);
#endif
#endif

#ifdef _HAVE_NI_LABWIN_CVI_
#include <time.h>
#include <utility.h>
#include <ansi/math.h>
#endif

extern  void	M_reverse_string(char *);
extern  void    M_get_rnd_seed(M_APM);

static	M_APM   M_rnd_aa;
static  M_APM   M_rnd_mm;
static  M_APM   M_rnd_XX;
static  M_APM   M_rtmp0;
static  M_APM   M_rtmp1;

static  int     M_firsttime2 = TRUE;

void	M_free_all_rnd()
{
if (M_firsttime2 == FALSE)
  {
   m_apm_free(M_rnd_aa);
   m_apm_free(M_rnd_mm);
   m_apm_free(M_rnd_XX);
   m_apm_free(M_rtmp0);
   m_apm_free(M_rtmp1);

   M_firsttime2 = TRUE;
  }
}

void	m_apm_set_random_seed(char *ss)
{
M_APM   btmp;

if (M_firsttime2)
  {
   btmp = M_get_stack_var();
   m_apm_get_random(btmp);
   M_restore_stack(1);
  }

m_apm_set_string(M_rnd_XX, ss);
}

void	m_apm_get_random(M_APM mrnd)
{

if (M_firsttime2)
  {
   M_firsttime2 = FALSE;

   M_rnd_aa = m_apm_init();
   M_rnd_XX = m_apm_init();
   M_rnd_mm = m_apm_init();
   M_rtmp0  = m_apm_init();
   M_rtmp1  = m_apm_init();

   m_apm_set_string(M_rnd_aa, "716805947629621");
   m_apm_set_string(M_rnd_mm, "1.0E15");

   M_get_rnd_seed(M_rnd_XX);
  }

m_apm_multiply(M_rtmp0, M_rnd_XX, M_rnd_aa);
m_apm_add(M_rtmp1, M_rtmp0, M_rnd_aa);
m_apm_integer_div_rem(M_rtmp0, M_rnd_XX, M_rtmp1, M_rnd_mm);
m_apm_copy(mrnd, M_rnd_XX);
mrnd->m_apm_exponent -= 15;
}

void	M_reverse_string(char *s)
{
int	ct;
char	ch, *p1, *p2;

if ((ct = strlen(s)) <= 1)
  return;

p1 = s;
p2 = s + ct - 1;
ct /= 2;

while (TRUE)
  {
   ch    = *p1;
   *p1++ = *p2;
   *p2-- = ch;

   if (--ct == 0)
     break;
  }
}

#ifndef _HAVE_NI_LABWIN_CVI_

#ifdef MSDOS

void	M_get_rnd_seed(M_APM mm)
{
int              millisec;
time_t 		 timestamp;
unsigned long    ul;
char             ss[32], buf1[48], buf2[32];
struct timeb     timebuffer;
M_APM		 atmp;

atmp = M_get_stack_var();

ftime(&timebuffer);

millisec  = (int)timebuffer.millitm;
timestamp = timebuffer.time;
ul        = (unsigned long)(timestamp / 7);
ul       += timestamp + 537;
strcpy(ss,ctime(&timestamp));

sprintf(buf1,"%d",(millisec / 10));
sprintf(buf2,"%lu",ul);

ss[0] = ss[18];
ss[1] = ss[17];
ss[2] = ss[15];
ss[3] = ss[14];
ss[4] = ss[12];
ss[5] = ss[11];
ss[6] = ss[9];
ss[7] = ss[23];
ss[8] = ss[20];
ss[9] = '\0';

M_reverse_string(buf2);
strcat(buf1,buf2);
strcat(buf1,ss);

m_apm_set_string(atmp, buf1);
atmp->m_apm_exponent = 15;
m_apm_integer_divide(mm, atmp, MM_One);

M_restore_stack(1);
}

#else

void	M_get_rnd_seed(M_APM mm)
{
unsigned long    sec3;
long             usec3;
char             buf1[32], buf2[32];
M_APM		 atmp;

atmp = M_get_stack_var();
M_get_microsec(&sec3,&usec3);

sprintf(buf1,"%ld",usec3);
sprintf(buf2,"%lu",sec3);
M_reverse_string(buf2);
strcat(buf1,buf2);

m_apm_set_string(atmp, buf1);
atmp->m_apm_exponent = 15;
m_apm_integer_divide(mm, atmp, MM_One);

M_restore_stack(1);
}

void	 M_get_microsec(unsigned long *sec, long *usec)
{
struct timeval time_now;
struct timezone time_zone;

gettimeofday(&time_now, &time_zone);

*sec  = time_now.tv_sec;
*usec = time_now.tv_usec;
}

#endif
#endif

#ifdef _HAVE_NI_LABWIN_CVI_

void	M_get_rnd_seed(M_APM mm)
{
double		 timer0;
int		 millisec;
char             *cvi_time, *cvi_date, buf1[64], buf2[32];
M_APM		 atmp;

atmp = M_get_stack_var();

cvi_date = DateStr();
cvi_time = TimeStr();
timer0   = Timer();

millisec = (int)(0.01 + 1000.0 * (timer0 - floor(timer0)));

sprintf(buf1, "%d", millisec);

buf2[0]  = cvi_time[6];
buf2[1]  = cvi_time[7];
buf2[2]  = cvi_time[3];
buf2[3]  = cvi_time[4];
buf2[4]  = cvi_time[0];
buf2[5]  = cvi_time[1];

buf2[6]  = cvi_date[3];
buf2[7]  = cvi_date[4];
buf2[8]  = cvi_date[0];
buf2[9]  = cvi_date[1];
buf2[10] = cvi_date[8];
buf2[11] = cvi_date[9];
buf2[12] = cvi_date[7];

buf2[13] = '4';
buf2[14] = '7';
buf2[15] = '\0';

strcat(buf1, buf2);

m_apm_set_string(atmp, buf1);
atmp->m_apm_exponent = 15;
m_apm_integer_divide(mm, atmp, MM_One);

M_restore_stack(1);
}

#endif
