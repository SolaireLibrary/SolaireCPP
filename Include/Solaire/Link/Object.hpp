#ifndef SOLAIRE_LINK_OBJECT_HPP
#define SOLAIRE_LINK_OBJECT_HPP

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
\file Object.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 9th November 2015
Last Modified	: 11th November 2015
*/

#include <cstdint>
#include "..\Core\Init.hpp"

namespace Solaire{ namespace Link{

	class SOLAIRE_EXPORT_API Object{
	private:
		Object(const Object&) = delete;
		Object(Object&&) = delete;
		Object& operator=(const Object&) = delete;
		Object& operator=(Object&&) = delete;
	protected:
		~Object()  throw() {}
	public:
		virtual void SOLAIRE_EXPORT_CALL Destructor() throw() = 0;
		virtual void SOLAIRE_EXPORT_CALL Free() throw() = 0;
	};
}}

#endif
