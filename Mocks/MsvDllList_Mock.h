

#ifndef MARSTECH_DLLLIST_MOCK_H
#define MARSTECH_DLLLIST_MOCK_H


#include "..\IMsvDllList.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvDllList_Mock:
	public IMsvDllList
{
public:
	MOCK_METHOD3(GetDll, MsvErrorCode(const char* id, std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator));
};


#endif // MARSTECH_DLLLIST_MOCK_H
