/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll List Implementation
* @details		Contains implementation @ref IMsvDllList of @ref IMsvDllList interface.
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


#ifndef MARSTECH_DLLLISTBASE_H
#define MARSTECH_DLLLISTBASE_H


#include "IMsvDllList.h"

#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <map>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech DLL List Implementation.
* @details	Implementation for MarsTech DLL list. Converts DLL id to path to DLL and decorator.
* @see		IMsvDllList
******************************************************************************************************/
class MsvDllList:
	public IMsvDllList
{
protected:
	/**************************************************************************************************//**
	* @brief		MarsTech DLL Data.
	* @details	DLL data storage - holds all required data for dynamic/shared library.
	******************************************************************************************************/
	class MsvDllData
	{
	public:
		/**************************************************************************************************//**
		* @brief			Constructor.
		* @param[in]	dllPath				Path to dynamic/shared library.
		* @param[in]	spDllDecorator		Shared pointer to decorator (it might be nullptr if decorator is not needed).
		******************************************************************************************************/
		MsvDllData(const char* dllPath, std::shared_ptr<IMsvDllDecorator> spDllDecorator = nullptr);

		/**************************************************************************************************//**
		* @brief			Deleted copy constructor.
		* @details		Copy constructor deleted -> copying is not allowed.
		* @param[in]	origin			Reference to copyied object.
		* @warning		Do not copy this object.
		******************************************************************************************************/
		MsvDllData(const MsvDllData& origin) = delete;

		/**************************************************************************************************//**
		* @brief			Deleted assign operator.
		* @details		Assign operator deleted -> assign is not allowed.
		* @param[in]	origin			Reference to assigned object.
		* @warning		Do not assign this object.
		******************************************************************************************************/
		MsvDllData& operator= (const MsvDllData& origin) = delete;

		/**************************************************************************************************//**
		* @brief			Get DLL data.
		* @details		Returns stored data for dynamic/shared library.
		* @param[out]	dllPath				Path to dynamic/shared library.
		* @param[out]	spDllDecorator		Shared pointer to decorator (it might be nullptr if decorator is not needed).
		******************************************************************************************************/
		void GetDllData(std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator) const;

	protected:
		/**************************************************************************************************//**
		* @brief		Path to DLL.
		* @details	Stores real path to dynamic/shared library.
		******************************************************************************************************/
		std::string m_dllPath;

		/**************************************************************************************************//**
		* @brief		DLL decorator.
		* @details	Share pointer to DLL/object decorator.
		* @note		It might be nullptr when decorator is not needed.
		******************************************************************************************************/
		std::shared_ptr<IMsvDllDecorator> m_spDllDecorator;
	};

public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	******************************************************************************************************/
	MsvDllList(std::shared_ptr<MsvLogger> spLogger = nullptr);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvDllList();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvDllList public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvDllList::GetDll(const char* id, std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator) const
	******************************************************************************************************/
	virtual MsvErrorCode GetDll(const char* id, std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator) const override;

	/*-----------------------------------------------------------------------------------------------------
	**											MsvDllList public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @brief			Add DLL data.
	* @details		Add DLL data definition (path, decorarator) for DLL id.
	* @param[in]	id										DLL id.
	* @param[in]	dllPath								Path to DLL.
	* @param[in]	spDllDecorator						Shared pointer to DLL/object decorator (it might be nullptr if decorator is not needed).
	* @retval		MSV_ALREADY_EXISTS_ERROR		When DLL id is already in DLL list.
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode AddDll(const char* id, const char* dllPath, std::shared_ptr<IMsvDllDecorator> spDllDecorator = nullptr);

protected:
	/**************************************************************************************************//**
	* @brief		DLL map.
	* @details	Contains data for each DLL id.
	* @see		MsvDllData
	******************************************************************************************************/
	std::map<std::string, std::shared_ptr<MsvDllData>> m_dlls;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // MARSTECH_DLL_H

/** @} */	//End of group MDLLFACTORY.
