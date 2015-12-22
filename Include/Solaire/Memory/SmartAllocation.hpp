#ifndef SOLAIRE_SMART_ALLOCATOR_HPP
#define SOLAIRE_SMART_ALLOCATOR_HPP

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
	\file SmartAllocator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 8th December 2015
	Last Modified	: 8th December 2015
*/

#include <memory>

namespace Solaire {

	class Allocator;

	template<class T>
	class UniqueAllocation {
	private:
		Allocator* mAllocator;
		T* mObject;
	private:
		bool DeleteObject() throw() {
			if(mObject == nullptr) return false;
			mObject->~T();
			if(! mAllocator->Deallocate(mObject)) return false;
			mObject = nullptr;
			return true;
		}

		UniqueAllocation(const UniqueAllocation<T>&) = delete;
		UniqueAllocation& operator=(const UniqueAllocation<T>&) = delete;
	public:
		UniqueAllocation() throw() :
			mAllocator(nullptr),
			mObject(nullptr)
		{}

		UniqueAllocation(Allocator& aAllocator, T* const aObject) throw() :
			mAllocator(&aAllocator),
			mObject(aObject)
		{}

		template<class T2>
		UniqueAllocation(UniqueAllocation<T2>&& aOther) throw() :
			mAllocator(aOther.mAllocator),
			mObject(aOther.ReleaseOwnership())
		{}

		~UniqueAllocation() throw() {
			DeleteObject();
		}

		template<class T2>
		UniqueAllocation& operator=(UniqueAllocation<T2>&& aOther) throw() {
			Swap(aOther);
			return *this;
		}

		void Swap(UniqueAllocation<T>& aOther) throw() {
			std::swap(mAllocator, aOther.mAllocator);
			std::swap(mObject, aOther.mObject);
		}

		T* ReleaseOwnership() throw() {
			T* const tmp = mObject;
			mObject = nullptr;
			return tmp;
		}

		Allocator& GetAllocator() const throw(){
			return *mAllocator;
		}

		operator bool() const throw() {
			return mObject != nullptr;
		}

		T& operator*() const throw() {
			return *mObject;
		}

		T* operator->() const throw() {
			return mObject;
		}
	}; 
	
	template<class T>
	class SharedAllocation {
	public:
		template<class T2>
		friend class SharedAllocation;
	private:
		uint32_t* mCount;
		Allocator* mAllocator;
		T* mObject;
	private:
		bool DeleteObject() throw() {
			if(mCount == nullptr) return false;
			-- *mCount;
			if(*mCount == 0) {
				mAllocator->Deallocate(mCount);
				mCount = nullptr;
			}else {
				return false;
			}

			if(mObject == nullptr) return false;
			mObject->~T();
			if(! mAllocator->Deallocate(mObject)) return false;
			mObject = nullptr;

			return true;
		}

	public:
		SharedAllocation() throw() :
			mCount(nullptr),
			mAllocator(nullptr),
			mObject(nullptr)
		{}

		SharedAllocation(Allocator& aAllocator, T* const aObject) throw() :
			mCount(new(aAllocator.Allocate(sizeof(uint32_t))) uint32_t(1)),
			mAllocator(&aAllocator),
			mObject(aObject)
		{}

		SharedAllocation(const SharedAllocation<T>& aOther) throw() :
			mCount(aOther.mCount),
			mAllocator(aOther.mAllocator),
			mObject(aOther.mObject)
		{
			if(mCount)++ *mCount;
		}

		SharedAllocation(SharedAllocation<T>&& aOther) throw() :
			mCount(aOther.mCount),
			mAllocator(aOther.mAllocator),
			mObject(aOther.mObject)
		{
			aOther.mObject = nullptr;
			aOther.mCount = nullptr;
		}

		template<class T2>
		SharedAllocation(const SharedAllocation<T2>& aOther) throw() :
			mCount(aOther.mCount),
			mAllocator(aOther.mAllocator),
			mObject(aOther.mObject)
		{
			++ *mCount;
		}

		template<class T2>
		SharedAllocation(SharedAllocation<T2>&& aOther) throw() :
			mCount(aOther.mCount),
			mAllocator(aOther.mAllocator),
			mObject(aOther.mObject)
		{
			aOther.mObject = nullptr;
			aOther.mCount = nullptr;
		}

		~SharedAllocation() throw() {
			DeleteObject();
		}

		SharedAllocation& operator=(const SharedAllocation<T>& aOther) throw() {
			DeleteObject();
			mCount = aOther.mCount;
			mAllocator = aOther.mAllocator;
			mObject = aOther.mObject;
			++ *mCount;
			return *this;
		}

		SharedAllocation& operator=(SharedAllocation<T>&& aOther) throw() {
			Swap(aOther);
			return *this;
		}

		template<class T2>
		SharedAllocation& operator=(const SharedAllocation<T2>& aOther) throw() {
			DeleteObject();
			mCount = aOther.mCount;
			mAllocator = aOther.mAllocator;
			mObject = aOther.mObject;
			++ *mCount;
			return *this;
		}

		void Swap(SharedAllocation<T>& aOther) throw() {
			std::swap(mAllocator, aOther.mAllocator);
			std::swap(mObject, aOther.mObject);
			std::swap(mCount, aOther.mCount);
		}

		T* ReleaseOwnership() throw() {
			if(mCount == nullptr) return nullptr;
			if(*mCount != 1) return nullptr;

			T* const tmp = mObject;
			mAllocator->Deallocate(mCount);
			mCount = nullptr;
			mObject = nullptr;
			return tmp;
		}

		Allocator& GetAllocator() const throw() {
			return *mAllocator;
		}

		uint32_t GetUserCount() const throw() {
			return mCount == nullptr ? 0 : *mCount;
		}

		operator bool() const throw() {
			return mObject != nullptr;
		}

		T& operator*() const throw() {
			return *mObject;
		}

		T* operator->() const throw() {
			return mObject;
		}
	};
}

#endif
