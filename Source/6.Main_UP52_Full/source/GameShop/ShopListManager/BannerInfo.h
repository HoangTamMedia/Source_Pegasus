
#pragma once

#include "include.h"
#include <map>

class CBannerInfo
{
public:
	CBannerInfo();
	virtual ~CBannerInfo();

	bool	SetBanner(std::string strdata, std::string strDirPath, bool bDonwLoad);

public:
	int		BannerSeq;
	char	BannerName[BANNER_LENGTH_NAME];
	char	BannerImageURL[INTERNET_MAX_URL_LENGTH];
	int		BannerOrder;
	int		BannerDirection;
	tm 		BannerStartDate;
	tm 		BannerEndDate;
	char	BannerLinkURL[INTERNET_MAX_URL_LENGTH];

	char	BannerImagePath[MAX_PATH];
};
