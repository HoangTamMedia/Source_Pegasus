
#include "stdafx.h"

#include "Public.h"
#include "ZzzOpenglUtil.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "ZzzTexture.h"
#include "ZzzAi.h"
#include "ZzzEffect.h"
#include "DSPlaySound.h"
#include "WSClient.h"
#include "CSResourceManager.h"

#ifdef RESOURCE_MANAGER

static  CSResourceManager   CSRManager;

CSResourceBuffer::CSResourceBuffer ()
{
    m_bFill = false;
    m_iCount= 0;
    memset ( m_iTexTypeList, 0, sizeof( int )*MAX_TEX_BUFFER*2 );
}

CSResourceBuffer::~CSResourceBuffer ()
{
}

void    CSResourceBuffer::Reset ( void )
{
    memset ( m_iTexTypeList, 0, sizeof( int )*m_iCount*2 );

    m_bFill     = false;
    m_iCount    = 0;
}

int     CSResourceBuffer::GetNum ( void )
{
    return m_iCount;
}

void    CSResourceBuffer::InsertResource ( int Type, int index )
{
    m_bFill = true;
    int offset;
    for ( int i=0; i<m_iCount; ++i )
    {

        if ( m_iTexTypeList[i][0]==Type )
        {
            offset = m_iTexTypeList[i][1];

            m_iTexTypeList[i][1]++;
            m_iIndexList[i][offset] = index;
            return;
        }
    }
    offset = m_iTexTypeList[m_iCount][1];

    m_iTexTypeList[m_iCount][0] = Type;
    m_iTexTypeList[m_iCount][1]++;
    m_iIndexList[m_iCount][offset] = index;

    m_iCount++;
}

void    CSResourceParticle::RenderList ( BYTE byRenderOneMore )
{
#ifdef RESOURCE_MANAGER
    int offset;
    int index;
    for ( int i=0; i<m_iCount; ++i )
    {
        offset = m_iTexTypeList[i][1];
        for ( int j=0; j<offset; ++j )
        {
            index = m_iIndexList[i][j];
            RenderParticle ( index, &Particles[index], byRenderOneMore );
        }
    }
#endif
}

void    CSResourceJoint ::RenderList ( BYTE byRenderOneMore )
{
#ifdef RESOURCE_MANAGER
    int offset;
    int index;
    for ( int i=0; i<m_iCount; ++i )
    {
        offset = m_iTexTypeList[i][1];
        for ( int j=0; j<offset; ++j )
        {
            index = m_iIndexList[i][j];
            RenderJoint ( index, &Joints[index], byRenderOneMore );
        }
    }
#endif
}

void    CSResourceEffect ::RenderList ( BYTE byRenderOneMore )
{
#ifdef RESOURCE_MANAGER
#endif
}

CSResourceManager::CSResourceManager ()
{

}

CSResourceManager::~CSResourceManager ()
{
}

void    CSResourceManager::Reset ( void )
{

}
#endif
