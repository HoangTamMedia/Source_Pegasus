
#pragma once

#include "include.h"

class CStringToken
{
public:
	CStringToken();
	virtual ~CStringToken();
	CStringToken(const std::string& dataLine, const std::string& delim);

	size_t countTokens();
	bool hasMoreTokens();
	std::string nextToken();

private:
	std::string data;
	std::string delimiter;
	std::vector<std::string> tokens;
	std::vector<std::string>::iterator index;

	void split();
	void IsNullString(std::string::size_type pos);
};
