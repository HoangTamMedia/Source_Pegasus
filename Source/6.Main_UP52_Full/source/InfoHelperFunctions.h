#pragma once

#include "Builder.h"

#ifdef INFO_BUILDER

namespace info
{

	class ExplanationInfo;
	class ItemInfo;
	class SkillInfo;
	class SlideInfo;
	class DialogInfo;
	class CreditInfo;

	class Builder;
}

inline
info::Builder&						TheBuilderInfo()
{
	assert( info::Builder::IsInitialized() );
	return g_BuilderINFO;
}

inline
const char*							TheGolbalText( int index )
{
	return TheBuilderInfo().QueryInfo( info::eInfo_Text_File )->getinfodataTEXT( index );
}

inline
const info::Script_Item*			TheItemText( int index )
{
	return TheBuilderInfo().QueryInfo( info::eInfo_Item_File )->getinfodataITEM( index );
}

inline
const info::Script_Skill*			TheSkillText( int index )
{
	return g_BuilderINFO.QueryInfo( info::eInfo_Skill_File )->getinfodataSKILL( index );
}

inline
const char*			TheSildeText( int level, int index )
{
	return TheBuilderInfo().QueryInfo( info::eInfo_Slide_File )->getinfodataSILDE( level, index );
}

inline
const info::Script_Credit*			TheCreditText( int index )
{
	return TheBuilderInfo().QueryInfo( info::eInfo_Credit_File )->getinfodataCREDIT( index );
}

inline
const info::Script_Dialog*			TheDialogText( int index )
{
	return g_BuilderINFO.QueryInfo( info::eInfo_Dialog_File )->getinfodataDIALOG( index );
}

#endif