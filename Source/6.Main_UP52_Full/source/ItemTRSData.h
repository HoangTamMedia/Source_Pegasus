#pragma once

struct CUSTOM_POSITION_INFO
{
	int		Index;
	int		ItemIndex;
	float	translationX;
	float	translationY;
	float	RotationX;
	float	RotationY;
	float	RotationZ;
	float	Scale;
};

class cCustomPosition
{
public:
	cCustomPosition();
	virtual ~cCustomPosition();
	void Init();
	void Load(CUSTOM_POSITION_INFO* info);

	bool GetItemTRSData(int ItemIndex);
	float GetItemTRSDataTransLationX(int ItemIndex);
	float GetItemTRSDataTransLationY(int ItemIndex);
	float GetItemTRSDataRotationX(int ItemIndex);
	float GetItemTRSDataRotationY(int ItemIndex);
	float GetItemTRSDataRotationZ(int ItemIndex);
	float GetItemTRSDataScale(int ItemIndex);

public:

	std::map<int, CUSTOM_POSITION_INFO> m_ItemTRSData;
};

extern cCustomPosition ItemTRSData;
