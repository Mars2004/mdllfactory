

#ifndef MARSTECH_DLLDECORATOR_MOCK_H
#define MARSTECH_DLLDECORATOR_MOCK_H


#include "..\IMsvDllDecorator.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvDllDecorator_Mock:
	public IMsvDllDecorator
{
public:
	MOCK_METHOD2(DecorateDllObject, MsvErrorCode(const char* id, std::shared_ptr<IMsvDllAdapter> spMsvDllAdapter));
};


#endif // MARSTECH_DLLDECORATOR_MOCK_H
