

#ifndef MARSTECH_DLL_MOCK_H
#define MARSTECH_DLL_MOCK_H


#include "..\IMsvDll.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvDll_Mock:
	public IMsvDll
{
public:
	MOCK_METHOD1(Initialize, MsvErrorCode(const char* dllPath));
	MOCK_METHOD0(Uninitialize, MsvErrorCode());
	MOCK_CONST_METHOD0(Initialized, bool());
	MOCK_METHOD3(GetDllObject, MsvErrorCode(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject, std::shared_ptr<IMsvDllDecorator> spDecorator = nullptr));
	MOCK_CONST_METHOD0(GetDllReferenceCount, std::int64_t());
};


#endif // MARSTECH_DLL_MOCK_H
