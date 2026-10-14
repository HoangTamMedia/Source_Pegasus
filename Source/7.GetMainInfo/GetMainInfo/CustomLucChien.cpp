#include "StdAfx.h"
#include "CustomLucChien.h"
#include "MemScript.h"
#include "pugixml.hpp"

CustomLucChien gCustomLucChien;

CustomLucChien::CustomLucChien()
{
	this->Init();
}


CustomLucChien::~CustomLucChien()
{
}

void CustomLucChien::Init() // OK
{
	for (int n = 0; n < MAX_CUSTOM_LUCCHIEN; n++)
	{
		this->mLucChienDataItem[n].Index = -1;
	}
}

void CustomLucChien::Load(char* FilePath) // OK
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(FilePath);
	if (res.status != pugi::status_ok)
	{
		CMemScript* lpMemScript = new CMemScript;
		//lpMemScript->ErrorMessageBox("File %s load fail. Error: %s", FilePath, res.description());
		return;
	}

	pugi::xml_node oCutomLucChien = file.child("CutomLucChien");
	pugi::xml_node ItemLucChien = oCutomLucChien.child("ItemLucChien");
	for (pugi::xml_node Item = ItemLucChien.child("Item"); Item; Item = Item.next_sibling())
	{
		ITEMDATA_LUCCHIEN info = { 0 };
		static int mIndex = 0;
		info.Index = mIndex++;
		info.IndexMin = Item.attribute("IndexMin").as_int();
		info.IndexMax = Item.attribute("IndexMax").as_int();
		info.LCCap = Item.attribute("LCCap").as_int();
		info.LCGoc = Item.attribute("LCGoc").as_int();
		info.LCLevel = Item.attribute("LCLevel").as_int();
		info.LCOptSkill = Item.attribute("LCOptSkill").as_int();
		info.LCOptLuck = Item.attribute("LCOptLuck").as_int();
		info.LCOptLife = Item.attribute("LCOptLife").as_int();
		info.LCOptExl = Item.attribute("LCOptExl").as_int();
		info.LCOptHarmony = Item.attribute("LCOptHarmony").as_int();
		info.LCOpt380 = Item.attribute("LCOpt380").as_int();
		info.LCOptAnc = Item.attribute("LCOptAnc").as_int();
		info.LCOptSocket = Item.attribute("LCOptSocket").as_int();
		this->SetInfo(info);
		
	}
}

void CustomLucChien::SetInfo(ITEMDATA_LUCCHIEN info) // OK
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_LUCCHIEN)
	{
		return;
	}

	this->mLucChienDataItem[info.Index] = info;
}
