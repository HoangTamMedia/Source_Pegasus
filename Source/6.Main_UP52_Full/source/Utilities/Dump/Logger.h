class CLogger
{
public:
	const static int MAXIMUM_TRY = 5;

	CLogger();
	~CLogger();

	bool				Open();
	void				Close();
	bool				Write(LPCTSTR szLine);
	bool				WriteWithFormat(LPCTSTR szFormat, ...);

private:
	FILE				*m_pFile;
	CRITICAL_SECTION	m_csLog;
	bool				m_bOpen;
};