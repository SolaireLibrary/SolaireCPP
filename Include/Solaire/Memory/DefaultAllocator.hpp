#ifndef SOLAIRE_DEFAULT_ALLOCATOR_HPP
#define SOLAIRE_DEFAULT_ALLOCATOR_HPP

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
	\file DefaultAllocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 25th September 2015
	Last Modified	: 15th November 2015
*/

#include "Allocator.hpp"

namespace Solaire{

	class DefaultAllocator : public Allocator {
	private:
		uint32_t mAllocatedBytes;
	public:
		DefaultAllocator();

		// Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL GetAllocatedBytes() const throw()  override;
		uint32_t SOLAIRE_EXPORT_CALL GetFreeBytes() const throw() override;

		void* SOLAIRE_EXPORT_CALL Allocate(const size_t aBytes) throw() override;
		bool SOLAIRE_EXPORT_CALL Deallocate(void* const aObject, const size_t aBytes) throw() override;
	};

	static DefaultAllocator DEFAULT_ALLOCATOR;

}

#endif