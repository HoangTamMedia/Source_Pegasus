#include "stdafx.h"
#include "CustomPet.h"
#include "NewUISystem.h"

cCustomPet gCustomPet2;

cCustomPet::cCustomPet()
{
	this->Init();
}

cCustomPet::~cCustomPet()
{

}

void cCustomPet::Init()
{
	this->m_CustomPetInfo.clear();
}

void cCustomPet::Load(CUSTOM_PET_INFO* info)
{
	for (int n = 0; n < MAX_PET_ITEM; n++)
	{
		if (info[n].Index < 0 || info[n].Index >= MAX_PET_ITEM)
		{
			return;
		}
		this->m_CustomPetInfo.insert(std::pair<int, CUSTOM_PET_INFO>(info[n].ItemIndex, info[n]));

	}
}

int cCustomPet::GetInfoByItem(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		if (it->second.PetType == 6)
		{
			return it->second.ItemIndex + 1171;
		}
	}
	return -1;
}

int cCustomPet::GetInfoByItemInventory(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		return it->second.IndexInventory + 1171;
	}
	return -1;
}

int cCustomPet::GetInfoPetType(int ItemIndex)
{

	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		return it->second.PetType;
	}
	return -1;
}

int cCustomPet::GetInfoPetEffect(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		return it->second.FenrrirEffect;
	}
	return 0;
}

float cCustomPet::GetScaleInv(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		return it->second.Scale_Inve;
	}
	return -1;
}

float cCustomPet::GetScaleDrop(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		return it->second.Scale_Drop;
	}
	return -1;
}

float cCustomPet::GetScaleView(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	float viewpet = 0;

	if (it != this->m_CustomPetInfo.end())
	{
		return it->second.Scale_View;
	}
	return -1;
}

float cCustomPet::GetScalePosX(int ItemIndex)
{

	for (std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.begin(); it != this->m_CustomPetInfo.end(); it++)
	{

		if (ItemIndex == it->second.ItemIndex  || ItemIndex == it->second.IndexInventory)
		{
			return it->second.Scale_X;
		}
	}

	return -1;

}

float cCustomPet::GetScalePosY(int ItemIndex)
{

	for (std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.begin(); it != this->m_CustomPetInfo.end(); it++)
	{

		if (ItemIndex == it->second.ItemIndex || ItemIndex == it->second.IndexInventory)
		{
			return it->second.Scale_Y;
		}
	}

	return -1;
}

float cCustomPet::GetScaleAngle(int ItemIndex)
{

	for (std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.begin(); it != this->m_CustomPetInfo.end(); it++)
	{

		if (ItemIndex == it->second.ItemIndex || ItemIndex == it->second.IndexInventory)
		{
			return it->second.Scale_Angle;
		}
	}

	return -1;

}

bool cCustomPet::CheckCustomPetByItem(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_CustomPetInfo.find(ItemIndex);

	if (it != this->m_CustomPetInfo.end())
	{
		return true;
	}
	return false;
}

void cCustomPet::PetFollowCharacter(int a1, float MaxPos)
{
	float v22;
	vec3_t MUFLOAT;
	vec3_t PetPos;
	vec3_t OwnerPos;
	vec3_t CurrentPos;
	int LootPosition = 1000;

	OwnerPos[0] = *(float*)(*(DWORD*)(a1 + 472) + 252);
	OwnerPos[1] = *(float*)(*(DWORD*)(a1 + 472) + 256);
	OwnerPos[2] = *(float*)(*(DWORD*)(a1 + 472) + 260);

	PetPos[0] = OwnerPos[0] - *(float*)(a1 + 252);
	PetPos[1] = OwnerPos[1] - *(float*)(a1 + 256);
	PetPos[2] = OwnerPos[2] - *(float*)(a1 + 260);

	CurrentPos[0] = *(float*)(a1 + 252);
	CurrentPos[1] = *(float*)(a1 + 256);
	CurrentPos[2] = *(float*)(a1 + 260);

	float PetMapPos = PetPos[0] * PetPos[0] + PetPos[1] * PetPos[1];

	float diff1 = ((OwnerPos[0] > CurrentPos[0]) ? OwnerPos[0] - CurrentPos[0] : CurrentPos[0] - OwnerPos[0]);
	float diff2 = ((OwnerPos[1] > CurrentPos[1]) ? OwnerPos[1] - CurrentPos[1] : CurrentPos[1] - OwnerPos[1]);

	float angle = atan(diff2 / diff1) * 180 / PI;
	float angle2 = atan2(diff1, diff2) * 180 / PI;

	if (PetMapPos >= MaxPos * MaxPos)
	{
		float speed;
		if (PetMapPos >= (MaxPos * 2) * (MaxPos * 2))
		{
			speed = 14.0f;
			speed += 1.0f;
			*(float*)(a1 + 120) = 0.89000002f;
		}
		else if (PetMapPos >= (MaxPos + (MaxPos / 2)) * (MaxPos + (MaxPos / 2)))
		{
			speed = 13.0f;
			*(float*)(a1 + 120) = 0.69000002f;
		}
		else
		{
			speed = 10.0f;
			*(float*)(a1 + 120) = 0.40000001f;
		}

		double MoveX = speed * (cos(angle * PI / 180));
		double MoveY = speed * (sin(angle * PI / 180));

		if (CurrentPos[0] < OwnerPos[0] && CurrentPos[1] < OwnerPos[1])
		{
			*(float*)(a1 + 252) += (float)MoveX;
			*(float*)(a1 + 256) += (float)MoveY;
			*(float*)(a1 + 272) = 180;
			*(float*)(a1 + 272) -= angle2;
		}
		else if (CurrentPos[0] < OwnerPos[0] && CurrentPos[1] > OwnerPos[1])
		{
			*(float*)(a1 + 252) += (float)MoveX;
			*(float*)(a1 + 256) -= (float)MoveY;
			*(float*)(a1 + 272) = angle2;
		}
		else if (CurrentPos[0] > OwnerPos[0] && CurrentPos[1] > OwnerPos[1])
		{
			*(float*)(a1 + 252) -= (float)MoveX;
			*(float*)(a1 + 256) -= (float)MoveY;
			*(float*)(a1 + 272) = 360;
			*(float*)(a1 + 272) -= angle2;
		}
		else if (CurrentPos[0] > OwnerPos[0] && CurrentPos[1] < OwnerPos[1])
		{
			*(float*)(a1 + 252) -= (float)MoveX;
			*(float*)(a1 + 256) += (float)MoveY;
			*(float*)(a1 + 272) = 180;
			*(float*)(a1 + 272) += angle2;
		}

		*(WORD*)(a1 + 18) = 2;
	}

}