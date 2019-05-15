

#ifndef MARSTECH_DLLADAPTER_MOCK_H
#define MARSTECH_DLLADAPTER_MOCK_H


#include "..\IMsvDllAdapter.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvDllAdapter_Mock:
	public IMsvDllAdapter
{
public:
	MOCK_METHOD2(GetDllAddress, MsvErrorCode(const char* dllAddressName, void*& pdllAddress));
	MOCK_CONST_METHOD0(Loaded, bool());
	MOCK_METHOD1(LoadDllLibrary, MsvErrorCode(const char* dllPath));
	MOCK_METHOD0(UnloadDllLibrary, MsvErrorCode());
};


#endif // MARSTECH_DLLADAPTER_MOCK_H
