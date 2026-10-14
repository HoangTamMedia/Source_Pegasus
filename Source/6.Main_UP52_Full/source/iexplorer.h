#ifndef _IEXPLORER_H_
#define _IEXPLORER_H_

#pragma once

namespace leaf {

	inline bool OpenExplorer(const std::string& url)
	{

		if(32 < (UINT)ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_NORMAL))
			return true;
		return false;
	}
}

#endif