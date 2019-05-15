/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Adapter Interface
* @details		Contains definition of @ref IMsvDllAdapter interface.
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


#ifndef MARSTECH_IDLLBASE_H
#define MARSTECH_IDLLBASE_H


#include "merror/MsvError.h"


/**************************************************************************************************//**
* @brief		MarsTech DLL Adapter Interface.
* @details	Interface for dynamic/shared libraries adapter. Wraps real (system) implementation
*				of dynamic/shared libraries.
* @see		MsvDll
******************************************************************************************************/
class IMsvDllAdapter
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDllAdapter() {}

	/**************************************************************************************************//**
	* @brief			Get DLL address.
	* @details		Gets dynamic/shared library address and returns it.
	* @param[in]	dllAddressName						Address name to get/load.
	* @param[out]	pdllAddress							Pointer to loaded address.
	* @retval		MSV_NOT_INITIALIZED_ERROR		When DLL library has not been loaded.
	* @retval		MSV_NOT_FOUND_ERROR				When DLL address was not found.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDllAddress(const char* dllAddressName, void*& pdllAddress) = 0;

	/**************************************************************************************************//**
	* @brief			Load check.
	* @details		Returns flag if DLL library is loaded (true) or not (false).
	* @retval		true		When loaded.
	* @retval		false		When not loaded.
	******************************************************************************************************/
	virtual bool Loaded() const = 0;

	/**************************************************************************************************//**
	* @brief			Load DLL library.
	* @details		Loads dynamic/shared library.
	* @param[in]	dllPath								Path to DLL library.
	* @retval		MSV_ALREADY_INITIALIZED_INFO	When DLL has been already loaded (this is info, not error).
	* @retval		MSV_OPEN_ERROR						When load DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode LoadDllLibrary(const char* dllPath) = 0;

	/**************************************************************************************************//**
	* @brief			Unload DLL library.
	* @details		Unloads dynamic/shared library.
	* @retval		MSV_NOT_INITIALIZED_INFO		When DLL library has not been loaded (this is info, not error).
	* @retval		MSV_CLOSE_ERROR					When unload DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	* @warning		All objects, functions, etc acquired from dynamic/shared library must be released before
	*					unloading dynamic/shared library.
	******************************************************************************************************/
	virtual MsvErrorCode UnloadDllLibrary() = 0;
};


#endif // MARSTECH_IDLLBASE_H

/** @} */	//End of group MDLLFACTORY.
