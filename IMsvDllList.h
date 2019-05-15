/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll List Interface
* @details		Contains definition of @ref IMsvDllList interface.
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


#ifndef MARSTECH_IDLLLIST_H
#define MARSTECH_IDLLLIST_H


#include "IMsvDllDecorator.h"

MSV_DISABLE_ALL_WARNINGS

#include <string>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech DLL List Interface.
* @details	Interface for dynamic/shared libraries list. Contains all dynamic/shared libraries
*				which may be loaded by process. Converts DLL id to path to DLL and also contains
*				shared pointer to decorator if needed.
* @see		MsvDllList
******************************************************************************************************/
class IMsvDllList
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDllList() {}

	/**************************************************************************************************//**
	* @brief			Get DLL data.
	* @details		Returns DLL data (path, decorarator) for DLL by it id.
	* @param[in]	id								DLL id.
	* @param[out]	dllPath						Path to DLL.
	* @param[out]	spDllDecorator				Shared pointer to DLL/object decorator.
	* @retval		other error code			When failed.
	* @retval		MSV_NOT_FOUND_ERROR		When DLL id was not found.
	* @retval		MSV_SUCCESS					On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDll(const char* id, std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator) const = 0;
};


#endif // MARSTECH_IDLLLIST_H

/** @} */	//End of group MDLLFACTORY.
