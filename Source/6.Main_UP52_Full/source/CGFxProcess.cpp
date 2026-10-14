#include "stdafx.h"

#ifdef LDK_ADD_SCALEFORM

#include "CGFxProcess.h"

#include "CGFxMainUi.h"
#include "CGFxInfoPopup.h"

#ifdef GFX_UI_TEST_CODE
	#include "CGFxSample.h"
#endif

#define UI_FILEPATH "./Data/Interface/GFx/"

static BYTE bBuxCode[3] = {0xfc,0xcf,0xab};
static void BuxConvert(BYTE *Buffer,int Size)
{
	for(int i=0;i<Size;i++)
		Buffer[i] ^= bBuxCode[i%3];
}

CGFXBase* GFxProcess::Find(GFxRegistType key)
{

	mapGFXContainer::iterator iter = m_mapGfxContainer.find(key);

	if( iter != m_mapGfxContainer.end() )
	{
		return (*iter).second;
	}

	return NULL;
}

GFxProcess* GFxProcess::GetInstancePtr()
{
	static GFxProcess* _pGFx = new GFxProcess();
	return _pGFx;
}

GFxProcess::GFxProcess()
{
	m_isCompletedLoad = FALSE;

	m_iNowSceneFlag = NON_SCENE;

	m_iWidth = 0;
	m_iHeight = 0;

	m_iUISelection = 0;

	m_bNoFontConfig = FALSE;
	m_FontConfigIndex = -1;
}

GFxProcess::~GFxProcess()
{
 	GFxDestroy();
}

void GFxProcess::OnCreateDevice(SInt bufw, SInt bufh, SInt left, SInt top, SInt w, SInt h, UInt flags)
{
	m_iWidth = w;
	m_iHeight = h;

	if(!m_isCompletedLoad)
		return;

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		if(temp->OnCreateDevice(bufw, bufh, left, top, w, h, flags))
		{

		}
	}
}

void GFxProcess::OnResetDevice()
{
	if(!m_isCompletedLoad)
		return;

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		if(temp->OnResetDevice())
		{

		}
	}
}

void GFxProcess::OnLostDevice()
{
	if(!m_isCompletedLoad)
		return;

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		if(temp->OnLostDevice())
		{

		}
	}
}

void GFxProcess::OnDestroyDevice()
{
	if(!m_isCompletedLoad)
		return;

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		if(temp->OnDestroyDevice())
		{

		}
	}
}

void GFxProcess::LoadFontConfigs(ConfigParser *parser)
{
	m_FontConfigs.Parse(parser);
	if (m_FontConfigs.GetSize() > 0)
	{
		m_FontConfigIndex = 0;
	}
	else
	{
		m_FontConfigIndex = -1;
	}
}
int GFxProcess::GetFontConfigIndexByName(const char* pname)
{
	for (UInt i = 0; i < m_FontConfigs.GetSize(); i++)
	{
		if (G_stricmp(pname, m_FontConfigs[i]->ConfigName) == 0)
			return i;
	}
	return -1;
}

FontConfig* GFxProcess::GetCurrentFontConfig()
{

	if (m_FontConfigIndex == -1)
		return NULL;

	FontConfig* fc = NULL;
	SInt sIdx = m_FontConfigIndex;
	bool ok = false;

	while (!ok)
	{
		ok = true;
		fc = m_FontConfigs[m_FontConfigIndex];

		for (UInt i=0; i < fc->FontLibFiles.GetSize(); i++)
		{

			GSysFile file(fc->FontLibFiles[i]);
			if (!file.IsValid())
			{
				ok = false;
				fprintf(stderr, "Fontlib file '%s' cannot be found. Skipping config '%s'..\n", fc->FontLibFiles[i].ToCStr(), fc->ConfigName.ToCStr());
				break;
			}
		}

		if (!ok)
		{
			m_FontConfigIndex++;
			m_FontConfigIndex %= (SInt)m_FontConfigs.GetSize();
			if (m_FontConfigIndex == sIdx)
				return NULL;
		}
	}

	return m_FontConfigs[m_FontConfigIndex];
}

bool GFxProcess::LoadDefaultFontConfigFromPath( const GString& path )
{
	if (!m_bNoFontConfig)
	{

		GString fontConfigFilePath;

		fontConfigFilePath.AppendString(path);
		if ( !GFxURLBuilder::ExtractFilePath(&fontConfigFilePath) )
		{
			fontConfigFilePath = "";
		}

		fontConfigFilePath += "fontconfig.txt";
		bool maintainIndex = false;

		if (m_FontConfigFilePath.GetLength() == 0)
		{
			GFileStat fileStats;
			if ( GSysFile::GetFileStat(&fileStats, fontConfigFilePath.ToCStr()) )
			{
				m_FontConfigFilePath = fontConfigFilePath;
				m_FontConfigFileStats = fileStats;
			}
		}
		else
		{
			if (fontConfigFilePath == m_FontConfigFilePath)
			{

				GFileStat fileStats;
				if ( GSysFile::GetFileStat(&fileStats, fontConfigFilePath.ToCStr()) )
				{
					if ( !(fileStats == m_FontConfigFileStats) )
					{
						m_FontConfigFileStats = fileStats;
						maintainIndex = true;
					}
				}
			}
		}

		ConfigParser parser(fontConfigFilePath.ToCStr());
		SInt oldIdx = m_FontConfigIndex;
		LoadFontConfigs(&parser);

		if ( maintainIndex &&
			(m_FontConfigIndex == 0) &&
			(oldIdx != -1) )
		{
			m_FontConfigIndex = oldIdx;
			m_FontConfigIndex %= (SInt)m_FontConfigs.GetSize();
		}

		return true;
	}
	return false;
}

bool GFxProcess::GFxInit()
{
 	if(!m_isCompletedLoad)
 	{
		GString filepath = UI_FILEPATH;
		GString filename = "";

		if (!LoadDefaultFontConfigFromPath(filepath) )
		{

		}

#ifdef ADD_SELETED_LANGUAGE_FONT

		m_FontConfigIndex = SELECTED_LANGUAGE;
		m_FontConfigIndex %= (SInt)m_FontConfigs.GetSize();
#endif

		FontConfig* pconfig = NULL;
		if (m_FontConfigIndex != -1)
		{
			pconfig = GetCurrentFontConfig();
		}

		const char* language = (m_FontConfigIndex == -1) ? "Default" : m_FontConfigs[m_FontConfigIndex]->ConfigName.ToCStr();

		filename = "mainFrame.gfx";
		CGFxMainUi* tempMainUI = CGFxMainUi::Make(GFxRegistType::eGFxRegist_MainUI, &m_mapGfxContainer, filepath+filename, GFxLoader::LoadAll | GFxLoader::LoadKeepBindData | GFxLoader::LoadWaitFrame1, 0 ,GFxRenderConfig::RF_EdgeAA | GFxRenderConfig::RF_StrokeMask, GFxMovieView::SM_NoScale, pconfig, language);
		if(tempMainUI == NULL)
		{
			delete tempMainUI;
			tempMainUI = NULL;

		}

		filename = "infoPopup.gfx";
		CGFxInfoPopup* tempPopup =  CGFxInfoPopup::Make(GFxRegistType::eGFxRegist_InfoPupup, &m_mapGfxContainer, filepath+filename, GFxLoader::LoadAll | GFxLoader::LoadKeepBindData | GFxLoader::LoadWaitFrame1, 0 ,GFxRenderConfig::RF_EdgeAA | GFxRenderConfig::RF_StrokeMask, GFxMovieView::SM_NoScale, pconfig, language);
		if(tempPopup == NULL)
		{
			delete tempPopup;
			tempPopup = NULL;

		}

#ifdef GFX_UI_TEST_CODE

		filename = "hpguege.gfx";
		GFxSample* tempImageTest = GFxSample::Make(GFxRegistType::eGFxRegist_ImageTest, &m_mapGfxContainer, filepath+filename, GFxLoader::LoadAll | GFxLoader::LoadKeepBindData | GFxLoader::LoadWaitFrame1, 0 ,GFxRenderConfig::RF_EdgeAA | GFxRenderConfig::RF_StrokeMask, GFxMovieView::SM_NoScale, pconfig);
		if(tempImageTest == NULL)
		{
			delete tempImageTest;
			tempImageTest = NULL;

		}
#endif

 		m_isCompletedLoad = TRUE;
 	}

	return m_isCompletedLoad;
}

void GFxProcess::GFxDestroy()
{

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter++)
	{
		CGFXBase* temp = (*iter).second;
		delete temp;
	}
	m_mapGfxContainer.clear();
}

void GFxProcess::GFxUpdate(int _scene)
{
	if(!m_isCompletedLoad)
		return;

	if(m_iNowSceneFlag != _scene)
	{
		for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
		{
			CGFXBase* temp = (*iter).second;

			switch (_scene)
			{
			case LOG_IN_SCENE:
				GFxSceneInit_LogIn((*iter).first, temp);
				break;

			case CHARACTER_SCENE:
				GFxSceneInit_Char((*iter).first, temp);
				break;

			case MAIN_SCENE:
				GFxSceneInit_Main((*iter).first, temp);
				break;
			}
		}
		m_iNowSceneFlag = _scene;
	}

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		switch (_scene)
		{
		case LOG_IN_SCENE:
			GFxSceneControll_LogIn((*iter).first, temp);
			break;

		case CHARACTER_SCENE:
			GFxSceneControll_Char((*iter).first, temp);
			break;

		case MAIN_SCENE:
			GFxSceneControll_Main((*iter).first, temp);
			break;
		}
	}
}

void GFxProcess::GFxRender()
{
	if(!m_isCompletedLoad)
		return;

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		if(temp->IsVisible())
		{

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_FRAGMENT_PROGRAM_ARB);
			glDisable(GL_VERTEX_PROGRAM_ARB);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if(temp->Render())
			{

			}

			glPopAttrib();

			EndBitmap();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glViewport2(0,0,m_iWidth,m_iHeight);
			gluPerspective2(1.f, (float)(m_iWidth)/(float)(m_iHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			GetOpenGLMatrix(CameraMatrix);
			EnableDepthTest();
			EnableDepthMask();

			glClear(GL_DEPTH_BUFFER_BIT);

			if(temp->RenderModel())
			{

			}

			UpdateMousePositionn();

			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			BeginBitmap();

		}
	}
}

void GFxProcess::GFxProcessEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool *pbNoFurtherProcessing)
{
	if(!m_isCompletedLoad)
		return;

	int mx = LOWORD(lParam), my = HIWORD(lParam);

	for( mapGFXContainer::iterator iter = m_mapGfxContainer.begin(); iter != m_mapGfxContainer.end(); iter ++ )
	{
		CGFXBase* temp = (*iter).second;

		if(temp->IsVisible())
		{
			GFxMovieView* pMove = temp->GetMovie();

			bool processedMouseEvent = false;
			if(uMsg == WM_MOUSEMOVE)
			{
				GFxMouseEvent mevent(GFxEvent::MouseMove, 0, (Float)mx, (Float)my);
				pMove->HandleEvent(mevent);
				processedMouseEvent = true;
			}
			else if(uMsg == WM_LBUTTONDOWN)
			{
				::SetCapture(hWnd);
				GFxMouseEvent mevent(GFxEvent::MouseDown, 0, (Float)mx, (Float)my);
				pMove->HandleEvent(mevent);
				processedMouseEvent = true;
			}
			else if(uMsg == WM_LBUTTONUP)
			{
				::ReleaseCapture();
				GFxMouseEvent mevent(GFxEvent::MouseUp, 0, (Float)mx, (Float)my);
				pMove->HandleEvent(mevent);
				processedMouseEvent = true;
			}
			else if(uMsg == WM_RBUTTONDOWN)
			{
				::SetCapture(hWnd);
				GFxMouseEvent mevent(GFxEvent::MouseDown, 1, (Float)mx, (Float)my);
				pMove->HandleEvent(mevent);
				processedMouseEvent = true;
			}
			else if(uMsg == WM_RBUTTONUP)
			{
				::ReleaseCapture();
				GFxMouseEvent mevent(GFxEvent::MouseUp, 1, (Float)mx, (Float)my);
				pMove->HandleEvent(mevent);
				processedMouseEvent = true;
			}

			if(processedMouseEvent && pMove->HitTest((Float)mx, (Float)my, GFxMovieView::HitTest_ShapesNoInvisible))
				*pbNoFurtherProcessing = true;

			if(uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP || uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_CHAR)
			{
				ProcessKeyEvent(pMove, uMsg, wParam, lParam);
			}
		}

	}
}

void GFxProcess::GFxSceneInit_LogIn(GFxRegistType _type, CGFXBase* _gfx)
{
	if(_type == GFxRegistType::eGFxRegist_MAX_COUNT)
	{

		return;
	}

	_gfx->SetUnLockVisible();

	switch(_type)
	{
	case GFxRegistType::eGFxRegist_NONE:

		break;

	case GFxRegistType::eGFxRegist_ImageTest:
		_gfx->SetVisible(TRUE);
		break;

	default:
		_gfx->SetVisible(FALSE);
		break;
	}
}

void GFxProcess::GFxSceneControll_LogIn(GFxRegistType _type, CGFXBase* _gfx)
{
	if(_type == GFxRegistType::eGFxRegist_MAX_COUNT)
	{

		return;
	}

	if(_gfx->IsVisible())
	{
		if(_gfx->Update())
		{

		}
	}
}

void GFxProcess::GFxSceneInit_Char(GFxRegistType _type, CGFXBase* _gfx)
{
	if(_type == GFxRegistType::eGFxRegist_MAX_COUNT)
	{

		return;
	}

	_gfx->SetUnLockVisible();

	switch(_type)
	{
	case GFxRegistType::eGFxRegist_NONE:

		break;

	default:
		_gfx->SetVisible(FALSE);
		break;
	}
}

void GFxProcess::GFxSceneControll_Char(GFxRegistType _type, CGFXBase* _gfx)
{
	if(_type == GFxRegistType::eGFxRegist_MAX_COUNT)
	{

		return;
	}

	if(_gfx->IsVisible())
	{
		if(_gfx->Update())
		{

		}
	}
}

void GFxProcess::GFxSceneInit_Main(GFxRegistType _type, CGFXBase* _gfx)
{
	if(_type == GFxRegistType::eGFxRegist_MAX_COUNT)
	{

		return;
	}

	_gfx->SetUnLockVisible();

	switch(_type)
	{
	case GFxRegistType::eGFxRegist_NONE:

		break;

	case GFxRegistType::eGFxRegist_MainUI:
	case GFxRegistType::eGFxRegist_InfoPupup:
		{

			_gfx->SetVisible(TRUE);
		}
		break;

	default:
		_gfx->SetVisible(FALSE);
		break;
	}

	if(_gfx->IsVisible())
	{
		if(_gfx->Init())
		{

		}
	}

}

void GFxProcess::GFxSceneControll_Main(GFxRegistType _type, CGFXBase* _gfx)
{
	if(_type == GFxRegistType::eGFxRegist_MAX_COUNT)
	{

		return;
	}

	if(_gfx->IsVisible())
	{
		if(_gfx->Update())
		{

		}
	}
}

void GFxProcess::SetSkillClearHotKey()
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
		temp->SetSkillClearHotKey();
}

void GFxProcess::SetSkillHotKey(int iHotKey, int _skillType)
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
		temp->SetSkillHotKey(iHotKey, _skillType);
}

int GFxProcess::GetSkillHotKey(int iHotKey)
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
	{
		return temp->GetSkillHotKey(iHotKey);
	}

	return -1;
}

void GFxProcess::SetItemClearHotKey()
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
		temp->SetItemClearHotKey();
}

void GFxProcess::SetItemHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
		temp->SetItemHotKey(iHotKey, iItemType, iItemLevel);
}

int GFxProcess::GetItemHotKey(int iHotKey)
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
	{
		return temp->GetItemHotKey(iHotKey);
	}

	return -1;
}

int GFxProcess::GetItemHotKeyLevel(int iHotKey)
{
	CGFxMainUi* temp = (CGFxMainUi*)Find(GFxRegistType::eGFxRegist_MainUI);

	if(temp)
	{
		return temp->GetItemHotKeyLevel(iHotKey);
	}

	return 0;
}

#endif
