#pragma once


struct CHECKITEMVIP
{
	int Index;
	int IndexItem;
	int Level;
	int LevelVip;
};


class CheckItemVip
{
public:
	CheckItemVip();
	~CheckItemVip();
	// ----
	void	Load();
	//====
	void	SkillFixData(char * FilePath);
	int CheckItemVip::CheckLevelVIP(int IndexItem, int ItemLevel);
	//===
	// ----Mapping
	std::map<int, CHECKITEMVIP> m_ItemVipCheck;
private:

	// ----
}; extern CheckItemVip gCheckItemVip;