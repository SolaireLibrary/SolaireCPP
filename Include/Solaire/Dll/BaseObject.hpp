#ifndef SOLAIRE_DLL_BASE_OBJECT_HPP
#define SOLAIRE_DLL_BASE_OBJECT_HPP

//Copyright 2015 Adam Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

// Contact :
// Email             : solairelibrary@mail.com
// GitHub repository : https://github.com/SolaireLibrary/SolaireCPP

/*!
\file BaseObject.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 9th November 2015
*/

#include <cstdint>
#include "Dll.inl"

namespace Solaire{namespace Dll{
	class SOLAIRE_DLL_API BaseObject{
	public:
		typedef uint32_t ErrorCode;
		
		enum : ErrorCode{
			NO_ERROR,
			RELEASED_UNREFERENCED_OBJECT,
			ERROR_OVERFLOW
		};
	private:
		BaseObject(const BaseObject&) = delete;
		BaseObject(BaseObject&&) = delete;
		BaseObject& operator=(const BaseObject&) = delete;
		BaseObject& operator=(BaseObject&&) = delete;
	public:
		virtual void CreateReference() = 0;
		virtual void ReleaseReference() = 0;
		virtual uint32_t GetReferenceCount() const = 0;
		virtual ErrorCode GetError() = 0;
	};
}}

#endif