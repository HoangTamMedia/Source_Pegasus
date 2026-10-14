
#if !defined(AFX_WAVEIO_H__A0449D63_0097_11D4_A092_A64970C5F176__INCLUDED_)
#define AFX_WAVEIO_H__A0449D63_0097_11D4_A092_A64970C5F176__INCLUDED_

#pragma once

#define INPUT	0
#define OUTPUT	1

class waveIO
{
public:
	waveIO(bool IO);
	virtual ~waveIO();
	bool LoadWaveHeader(char *szFilename);
	bool WriteWaveHeader(char *szFilename, PCMWAVEFORMAT wf, int nWaveDateSize);
	bool CloseWaveFile();
	WAVEFORMATEX GetWaveFormatEx() {return m_wfex;}
	bool ReadWaveData(char *buffer, int nSizeOfBuffer);
	bool WriteWaveData(char *buffer, int nSizeOfBuffer);
	int GetDataSize() {return m_DataSize;}
private:
	HMMIO			m_hmmio;
	WAVEFORMATEX	m_wfex;
	int				m_DataSize;
	int				m_DataLeft;
	int				m_SilentSample;
	bool			m_IO;
};

#endif
