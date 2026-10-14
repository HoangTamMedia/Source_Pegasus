#ifndef CRYPTOPP_WAIT_H
#define CRYPTOPP_WAIT_H

#include "config.h"

#ifdef SOCKETS_AVAILABLE

#include "misc.h"
#include "cryptlib.h"
#include <vector>

#ifdef USE_WINDOWS_STYLE_SOCKETS
#include <winsock2.h>
#else
#include <sys/types.h>
#endif

#include "hrtimer.h"

NAMESPACE_BEGIN(CryptoPP)

class Tracer
{
public:
	Tracer(unsigned int level) : m_level(level) {}
	virtual ~Tracer() {}

protected:

	virtual void Trace(unsigned int n, std::string const& s) = 0;

	virtual bool UsingDefaults() const { return true; }

protected:
	unsigned int m_level;

	void TraceIf(unsigned int n, std::string const&s)
		{ if (n) Trace(n, s); }

	unsigned int Tracing(unsigned int nr, unsigned int minLevel) const
		{ return (UsingDefaults() && m_level >= minLevel) ? nr : 0; }
};

#define CRYPTOPP_TRACER_CONSTRUCTOR(DERIVED) \
	public: DERIVED(unsigned int level = 0) : Tracer(level) {}

#define CRYPTOPP_BEGIN_TRACER_CLASS_1(DERIVED, BASE1) \
	class DERIVED : virtual public BASE1 { CRYPTOPP_TRACER_CONSTRUCTOR(DERIVED)

#define CRYPTOPP_BEGIN_TRACER_CLASS_2(DERIVED, BASE1, BASE2) \
	class DERIVED : virtual public BASE1, virtual public BASE2 { CRYPTOPP_TRACER_CONSTRUCTOR(DERIVED)

#define CRYPTOPP_END_TRACER_CLASS };

#define CRYPTOPP_BEGIN_TRACER_EVENTS(UNIQUENR)	enum { EVENTBASE = UNIQUENR,
#define CRYPTOPP_TRACER_EVENT(EVENTNAME)				EventNr_##EVENTNAME,
#define CRYPTOPP_END_TRACER_EVENTS				};

#define CRYPTOPP_TRACER_EVENT_METHODS(EVENTNAME, LOGLEVEL) \
	virtual unsigned int Trace##EVENTNAME() const { return Tracing(EventNr_##EVENTNAME, LOGLEVEL); } \
	virtual void Trace##EVENTNAME(std::string const& s) { TraceIf(Trace##EVENTNAME(), s); }

class CallStack
{
public:
	CallStack(char const* i, CallStack const* p) : m_info(i), m_prev(p) {}
	CallStack const* Prev() const { return m_prev; }
	virtual std::string Format() const;

protected:
	char const* m_info;
	CallStack const* m_prev;
};

class CallStackWithNr : public CallStack
{
public:
	CallStackWithNr(char const* i, word32 n, CallStack const* p) : CallStack(i, p), m_nr(n) {}
	std::string Format() const;

protected:
	word32 m_nr;
};

class CallStackWithStr : public CallStack
{
public:
	CallStackWithStr(char const* i, char const* z, CallStack const* p) : CallStack(i, p), m_z(z) {}
	std::string Format() const;

protected:
	char const* m_z;
};

CRYPTOPP_BEGIN_TRACER_CLASS_1(WaitObjectsTracer, Tracer)
	CRYPTOPP_BEGIN_TRACER_EVENTS(0x48752841)
		CRYPTOPP_TRACER_EVENT(NoWaitLoop)
	CRYPTOPP_END_TRACER_EVENTS
	CRYPTOPP_TRACER_EVENT_METHODS(NoWaitLoop, 1)
CRYPTOPP_END_TRACER_CLASS

struct WaitingThreadData;

class WaitObjectContainer : public NotCopyable
{
public:

	class Err : public Exception
	{
	public:
		Err(const std::string& s) : Exception(IO_ERROR, s) {}
	};

	static unsigned int MaxWaitObjects();

	WaitObjectContainer(WaitObjectsTracer* tracer = 0);

	void Clear();
	void SetNoWait(CallStack const& callStack);
	void ScheduleEvent(double milliseconds, CallStack const& callStack);

	bool Wait(unsigned long milliseconds);

#ifdef USE_WINDOWS_STYLE_SOCKETS
	~WaitObjectContainer();
	void AddHandle(HANDLE handle, CallStack const& callStack);
#else
	void AddReadFd(int fd, CallStack const& callStack);
	void AddWriteFd(int fd, CallStack const& callStack);
#endif

private:
	WaitObjectsTracer* m_tracer;

#ifdef USE_WINDOWS_STYLE_SOCKETS
	void CreateThreads(unsigned int count);
	std::vector<HANDLE> m_handles;
	std::vector<WaitingThreadData *> m_threads;
	HANDLE m_startWaiting;
	HANDLE m_stopWaiting;
#else
	fd_set m_readfds, m_writefds;
	int m_maxFd;
#endif
	bool m_noWait;
	double m_firstEventTime;
	Timer m_eventTimer;

#ifdef USE_WINDOWS_STYLE_SOCKETS
	typedef size_t LastResultType;
#else
	typedef int LastResultType;
#endif
	enum { LASTRESULT_NOWAIT = -1, LASTRESULT_SCHEDULED = -2, LASTRESULT_TIMEOUT = -3 };
	LastResultType m_lastResult;
	unsigned int m_sameResultCount;
	Timer m_noWaitTimer;
	void SetLastResult(LastResultType result);
	void DetectNoWait(LastResultType result, CallStack const& callStack);
};

NAMESPACE_END

#endif

#endif
