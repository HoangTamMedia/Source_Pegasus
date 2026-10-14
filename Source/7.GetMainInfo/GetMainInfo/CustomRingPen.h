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

class cCustomRingPen
{
public:
	cCustomRingPen();
	virtual ~cCustomRingPen();
	void Init();
	bool Load(char* path);
	void SetInfo(CUSTOM_RING_INFO info);
public:
	CUSTOM_RING_INFO m_CustomRingPenInfo[MAX_CUSTOM_RP];
}; extern cCustomRingPen gCustomRingPen;