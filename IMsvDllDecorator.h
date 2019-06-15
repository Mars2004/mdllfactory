/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Decorator Interface
* @details		Contains definition of @ref IMsvDllDecorator interface.
* @author		Martin Svoboda
* @date			06.04.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Dll Factory.

MarsTech Dependency Injection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MarsTech Promise Like Syntax is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef MARSTECH_IDLLDECORATOR_H
#define MARSTECH_IDLLDECORATOR_H


#include "IMsvDllAdapter.h"
#include "IMsvDllObject.h"

MSV_DISABLE_ALL_WARNINGS

#include <memory>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech DLL Decorator Interface.
* @details	Interface for DLL decorators. It decorates DLLs without exported function GetDllObject
*				(probably C DLL, third party DLL, etc.) which can be handled by @ref IMsvDll.
* @see		IMsvDllObject
* @note		Just inherit from it when you need to load some DLL without exported function GetDllObject
*				and use it in your implementation of @ref IMsvDllList.
******************************************************************************************************/
class IMsvDllDecorator:
	public IMsvDllObject
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDllDecorator() {}

	/**************************************************************************************************//**
	* @brief			Decorate DLL object.
	* @details		Decorates dynamic/shared object or interface.
	* @param[in]	id										Path to DLL library.
	* @param[in]	spMsvDllAdapter					DLL adapter to decorate.
	* @retval		other_error_code					When decoration failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode DecorateDllObject(const char* id, std::shared_ptr<IMsvDllAdapter> spMsvDllAdapter) = 0;
};


#endif // MARSTECH_IDLLDECORATOR_H

/** @} */	//End of group MDLLFACTORY.
