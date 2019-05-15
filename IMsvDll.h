/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Interface
* @details		Contains definition of @ref IMsvDll interface.
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


#ifndef MARSTECH_IDLL_H
#define MARSTECH_IDLL_H


#include "IMsvDllDecorator.h"


/**************************************************************************************************//**
* @brief		MarsTech DLL Interface.
* @details	Interface for dynamic/shared libraries.
* @see		MsvDll
******************************************************************************************************/
class IMsvDll
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDll() {}

	/**************************************************************************************************//**
	* @brief			Initialize DLL library.
	* @details		Initializes dynamic/shared library. It also loads dynamic/shared library.
	* @param[in]	dllPath								Path to DLL library.
	* @retval		MSV_ALREADY_INITIALIZED_INFO	When DLL has been already initialized (this is info, not error).
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_OPEN_ERROR						When load DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const char* dllPath) = 0;

	/**************************************************************************************************//**
	* @brief			Uninitialize DLL library.
	* @details		Uninitializes dynamic/shared library and unloads it.
	* @retval		MSV_NOT_INITIALIZED_INFO		When DLL library has not been initialized (this is info, not error).
	* @retval		MSV_CLOSE_ERROR					When unload DLL library failed.
	* @retval		MSV_SUCCESS							On success.
	* @warning		Libraries with reference count greater then 0 should not be unitialized. It might lead
	*					to crash. On the other way, some DLLs might hold its acquired objects in shared_ptr
	*					too. Uninitialize just logs warning when uninitializing dynamic/shared library
	*					with references, but unloads it anyway.
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() = 0;

	/**************************************************************************************************//**
	* @brief			Initialize check.
	* @details		Returns flag if DLL library is initialized and successfully loaded (true) or not (false).
	* @retval		true		When initialized.
	* @retval		false		When not initialized.
	******************************************************************************************************/
	virtual bool Initialized() const = 0;

	/**************************************************************************************************//**
	* @brief			Get DLL object.
	* @details		Checks if object is already acquired (DLL stores weak_ptr of all acquired objects)
	*					and return it when it is acquired. Acquires object from DLL when it is not acquired.
	* @retval		MSV_NOT_INITIALIZED_ERROR		When DLL library has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR				When DLL entry point or DLL object was not found.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject, std::shared_ptr<IMsvDllDecorator> spDecorator = nullptr) = 0;

	/**************************************************************************************************//**
	* @brief			Get reference count of DLL objects.
	* @details		Returns number of references to DLL objects (objects acquired from a dynamic/shared library).
	* @returns		int64_t	Number of references to DLL objects.
	******************************************************************************************************/
	virtual std::int64_t GetDllReferenceCount() const = 0;
};


#endif // MARSTECH_IDLL_H

/** @} */	//End of group MDLLFACTORY.
