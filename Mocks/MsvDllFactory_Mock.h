

#ifndef MARSTECH_DLLFACTORY_MOCK_H
#define MARSTECH_DLLFACTORY_MOCK_H


#include "..\IMsvDllFactory.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvDllFactory_Mock:
	public IMsvDllFactory
{
public:
	MOCK_METHOD2(GetDll, MsvErrorCode(const char* id, std::shared_ptr<IMsvDll>& spDll));
	MOCK_METHOD2(GetDllObject, MsvErrorCode(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject));
	MOCK_METHOD1(ReleaseDll, MsvErrorCode(const char* id));
};


#endif // MARSTECH_DLLFACTORY_MOCK_H
