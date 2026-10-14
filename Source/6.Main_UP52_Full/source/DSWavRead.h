
#ifndef WAVE_READ_H
#define WAVE_READ_H

#include <mmreg.h>

class CWaveSoundRead
{
public:
    WAVEFORMATEX* m_pwfx;
    HMMIO         m_hmmioIn;
    MMCKINFO      m_ckIn;
    MMCKINFO      m_ckInRiff;

public:
    CWaveSoundRead();
    ~CWaveSoundRead();

    HRESULT Open( CHAR* strFilename );
    HRESULT Reset();
    HRESULT Read( UINT nSizeToRead, BYTE* pbData, UINT* pnSizeRead );
    HRESULT Close();

};

#endif WAVE_READ_H
