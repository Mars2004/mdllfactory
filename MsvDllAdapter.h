/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Adapter Implementation
* @details		Contains implementation @ref MsvDllAdapter of @ref IMsvDllAdapter interface.
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


#ifndef MARSTECH_DLLOBJECT_H
#define MARSTECH_DLLOBJECT_H


#include "IMsvDllAdapter.h"

#include "mlogging\mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif //_WIN32

#include <mutex>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech DLL Adapter Implementation.
* @details	Implementation for MarsTech dynamic/shared library adapter. Wraps real (system) implementation
*				of dynamic/shared libraries.
* @see		IMsvDllAdapter
******************************************************************************************************/
class MsvDllAdapter:
	public IMsvDllAdapter
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	******************************************************************************************************/
	MsvDllAdapter(std::shared_ptr<MsvLogger> spLogger = nullptr);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvDllAdapter();

	/**************************************************************************************************//**
	* @brief			Deleted copy constructor.
	* @details		Copy constructor deleted -> copying is not allowed.
	* @param[in]	origin			Reference to copyied object.
	* @warning		Do not copy this object.
	******************************************************************************************************/
	MsvDllAdapter(const MsvDllAdapter& origin) = delete;

	/**************************************************************************************************//**
	* @brief			Deleted assign operator.
	* @details		Assign operator deleted -> assign is not allowed.
	* @param[in]	origin			Reference to assigned object.
	* @warning		Do not assign this object.
	******************************************************************************************************/
	MsvDllAdapter& operator= (const MsvDllAdapter& origin) = delete;

	/**************************************************************************************************//**
	* @copydoc IMsvDllAdapter::GetDllAddress(const char* dllAddressName, void*& pdllAddress)
	******************************************************************************************************/
	virtual MsvErrorCode GetDllAddress(const char* dllAddressName, void*& pdllAddress) override;

	/**************************************************************************************************//**
	* @copydoc IMsvDllAdapter::Loaded() const
	******************************************************************************************************/
	virtual bool Loaded() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDllAdapter::LoadDllLibrary(const char* dllPath)
	******************************************************************************************************/
	virtual MsvErrorCode LoadDllLibrary(const char* dllPath) override;

	/**************************************************************************************************//**
	* @copydoc IMsvDllAdapter::UnloadDllLibrary()
	******************************************************************************************************/
	virtual MsvErrorCode UnloadDllLibrary() override;

protected:
	/**************************************************************************************************//**
	* @brief		Thread pool mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

#ifdef _WIN32
	/**************************************************************************************************//**
	* @brief		Pointer to system DLL.
	* @details	Pointer to system represention of dynamic library.
	******************************************************************************************************/
	HINSTANCE m_pHandle;
#else
	/**************************************************************************************************//**
	* @brief		Pointer to system shared library.
	* @details	Pointer to system represention of shared library.
	******************************************************************************************************/
	void* m_pHandle;
#endif //_WIN32

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // MARSTECH_DLLOBJECT_H

/** @} */	//End of group MDLLFACTORY.
