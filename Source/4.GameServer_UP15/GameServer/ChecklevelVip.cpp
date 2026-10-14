#include "stdafx.h"
#include "ChecklevelVip.h"
#include "GameMain.h"
#include "Util.h"
#include "Path.h"
#include "Util.h"
#include "pugixml.hpp"

CheckItemVip gCheckItemVip;
// -------------------------------------------------------------------------------

CheckItemVip::CheckItemVip()
{

}
// -------------------------------------------------------------------------------

CheckItemVip::~CheckItemVip()
{

}
// -------------------------------------------------------------------------------

void CheckItemVip::Load()
{
	this->m_ItemVipCheck.clear();


	this->SkillFixData(gPath.GetFullPath("Item\\CheckItemVip.xml")); //Fix Skill Tele

}
// -------------------------------------------------------------------------------

void CheckItemVip::SkillFixData(char * FilePath)
{

	int sk_Index = 0;
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(FilePath);
	if (res.status != pugi::status_ok){
		ErrorMessageBox("File %s load fail. Error: %s", FilePath, res.description());
		return;
	}
	//--
	pugi::xml_node oCheckItemVip = file.child("CheckItemVip");
	//--

	pugi::xml_node eSkillSuDungKhiTele = oCheckItemVip.child("ConfigCheckVip");
	for (pugi::xml_node rSkillSuDungKhiTele = eSkillSuDungKhiTele.child("Item"); rSkillSuDungKhiTele; rSkillSuDungKhiTele = rSkillSuDungKhiTele.next_sibling()){
		CHECKITEMVIP info;
		info.Index = sk_Index;
		info.IndexItem = rSkillSuDungKhiTele.attribute("IndexItem").as_int();
		info.Level = rSkillSuDungKhiTele.attribute("LevelItem").as_int();
		info.LevelVip = rSkillSuDungKhiTele.attribute("LevelVip").as_int();
		this->m_ItemVipCheck.insert(std::pair<int, CHECKITEMVIP>(info.IndexItem, info));
		sk_Index++;
	}
	
}

int CheckItemVip::CheckLevelVIP(int IndexItem , int ItemLevel) 
{
	for (int i = 0; i < this->m_ItemVipCheck.size(); i++)
	{
		
		if (this->m_ItemVipCheck[i].IndexItem == IndexItem )
		{
			
			return this->m_ItemVipCheck[i].LevelVip;
		}

	}
	return 0;
}

