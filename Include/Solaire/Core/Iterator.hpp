#ifndef SOLAIRE_ITERATOR_HPP
#define SOLAIRE_ITERATOR_HPP

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
	\file Iterator.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 3rd December 2015
	Last Modified	: 3rd December 2015
*/

#include <cstdint>
#include "Init.hpp"

namespace Solaire {

	class BaseIterator {
	public:
		typedef int32_t Offset;
	protected:
		virtual Offset SOLAIRE_EXPORT_CALL GetOffset() const throw() = 0;
	public:
		inline bool operator==(const BaseIterator& aOther) const throw() {
			return GetOffset() == aOther.GetOffset();
		}

		inline bool operator!=(const BaseIterator& aOther) const throw() {
			return GetOffset() != aOther.GetOffset();
		}

		inline bool operator<(const BaseIterator& aOther) const throw() {
			return GetOffset() < aOther.GetOffset();
		}

		inline bool operator>(const BaseIterator& aOther) const throw() {
			return GetOffset() > aOther.GetOffset();
		}

		inline bool operator<=(const BaseIterator& aOther) const throw() {
			return GetOffset() <= aOther.GetOffset();
		}

		inline bool operator>=(const BaseIterator& aOther) const throw() {
			return GetOffset() >= aOther.GetOffset();
		}

		inline Offset operator-(const BaseIterator& aOther) const throw() {
			return GetOffset() - aOther.GetOffset();
		}
	};

	template<class T>
	class Iterator : public BaseIterator {
	public:
		typedef T Type;
	public:
		virtual Type* SOLAIRE_EXPORT_CALL operator->() throw() = 0;
		virtual Iterator<Type>& SOLAIRE_EXPORT_CALL operator++() throw() = 0;
		virtual Iterator<Type>& SOLAIRE_EXPORT_CALL operator--() throw() = 0;
		virtual Iterator<Type>& SOLAIRE_EXPORT_CALL operator+=(const Offset) throw() = 0;
		virtual Iterator<Type>& SOLAIRE_EXPORT_CALL operator-=(const Offset) throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~Iterator(){}

		inline Type& operator*() throw() {
			return *operator->();
		}
	};

	template<class ITERATOR, class T>
	class IteratorSTL : public Iterator<T>{
	private:
		const ITERATOR mBegin;
		ITERATOR mCurrent;
	protected:
		//Inherited from Iterator

		Offset SOLAIRE_EXPORT_CALL GetOffset() const throw() override {
			return mCurrent - mBegin;
		}
	public:
		IteratorSTL(const ITERATOR aCurrent) :
			mBegin(aCurrent),
			mCurrent(aCurrent)
		{}

		IteratorSTL(const ITERATOR aBegin, const Offset aOffset) :
			mBegin(aBegin),
			mCurrent(aBegin + aOffset)
		{}

		SOLAIRE_EXPORT_CALL ~IteratorSTL() {

		}

		// Inherited from Iterator

		Type* SOLAIRE_EXPORT_CALL operator->() throw() {
			return &*mCurrent;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator++() throw() {
			++mCurrent;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator--() throw() {
			--mCurrent;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator+=(const Offset aOffset) throw() {
			mCurrent += aOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator-=(const Offset aOffset) throw() override {
			mCurrent -= aOffset;
			return *this;
		}
	}; 
	
	template<class T>
	using IteratorPtr = IteratorSTL<T*, T>;

	template<class ITERATOR, class T, class T2>
	class DereferenceIterator : public Iterator<T2>{
	private:
		ITERATOR mIterator;
	protected:
		//Inherited from Iterator
		Offset SOLAIRE_EXPORT_CALL GetOffset() const throw() override{
			return mIterator.GetOffset();
		}
	public:
		DereferenceIterator(const ITERATOR aIterator) :
			mIterator(aIterator)
		{}

		SOLAIRE_EXPORT_CALL ~DereferenceIterator() {

		}

		// Inherited from Iterator

		Type* operator->() throw() {
			return &*mIterator;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator++() throw() {
			++mIterator;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator--() throw() {
			--mIterator;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator+=(const Offset aOffset) throw() {
			mIterator += aOffset;
			return *this;
		}

		Iterator<Type>& SOLAIRE_EXPORT_CALL operator-=(const Offset aOffset) throw() override {
			mIterator -= aOffset;
			return *this;
		}
	};
}


#endif
