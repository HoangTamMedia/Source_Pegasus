#pragma once
#define MAX_CUSTOM_RP 100

struct CUSTOM_RING_INFO
{
	int Index;
	int ItemIndex;
	char ModelName[32];
	int ItemType;
	int ColorR;
	int ColorG;
	int ColorB;
};

class CCustomRingPen
{
public:
	CCustomRingPen();
	virtual ~CCustomRingPen();
	void Init();
	void Load(CUSTOM_RING_INFO* info);
	bool CheckCustomRing(int ItemIndex);
	bool CheckCustomPen(int ItemIndex);
	int CheckCustomRingPen(int ItemIndex);
public:
	std::map<int, CUSTOM_RING_INFO> m_CustomRingPenInfo;
};

extern CCustomRingPen gCustomRingPen;