#ifndef SOLAIRE_ENCODE_JSON_HPP
#define SOLAIRE_ENCODE_JSON_HPP

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
	\file Json.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 23rd November 2015
	Last Modified	: 23rd November 2015
*/

#include "..\Core\Init.hpp"
#include "..\Strings\StringFragment.hpp"
#include "..\IO\Stream.hpp"
#include "Value.hpp"

namespace Solaire{ 
	
	struct Json {
		class SOLAIRE_EXPORT_API Reader {
		public:
			virtual bool SOLAIRE_EXPORT_CALL BeginArray() throw() = 0;
			virtual bool SOLAIRE_EXPORT_CALL EndArray() throw() = 0;

			virtual bool SOLAIRE_EXPORT_CALL BeginObject() throw() = 0;
			virtual bool SOLAIRE_EXPORT_CALL EndObject() throw() = 0;

			virtual bool SOLAIRE_EXPORT_CALL MemberName(const ConstStringFragment) throw() = 0;

			virtual bool SOLAIRE_EXPORT_CALL ValueNull() throw() = 0;
			virtual bool SOLAIRE_EXPORT_CALL ValueBool(const bool) throw() = 0;
			virtual bool SOLAIRE_EXPORT_CALL ValueNumber(const double) throw() = 0;
			virtual bool SOLAIRE_EXPORT_CALL ValueString(const ConstStringFragment) throw() = 0;
		};

		class Writer {
		private:
			enum ContainterType : uint8_t {
				STATE_ARRAY,
				STATE_OBJECT
			};
			struct State {
				ContainterType type;
				DynamicArray<String> names;
				DynamicArray<String> values;

				State(Allocator&);
			};
		private:
			WriteStream& mOutputStream;
			DynamicArray<State> mState;
		private:
			bool AddValueInternal(const ConstStringFragment) throw();
		public:
			Writer(WriteStream&);
			Writer(Allocator& aAllocator, WriteStream&);
			~Writer();

			bool IsArray() const throw();
			bool IsObject() const throw();

			bool AddName(const ConstStringFragment);
			bool EndArray() throw();
			bool EndObject() throw();

			bool BeginArray() throw();
			bool BeginObject() throw();

			bool AddNull() throw();
			bool AddBool(const bool) throw();
			bool AddNumber(const double) throw();
			bool AddString(const ConstStringFragment) throw();

			bool BeginArray(const ConstStringFragment) throw();
			bool BeginObject(const ConstStringFragment) throw();

			bool AddNull(const ConstStringFragment) throw();
			bool AddNumber(const ConstStringFragment) throw();
			bool AddBool(const ConstStringFragment, const bool) throw();
			bool AddNumber(const ConstStringFragment, const double) throw();
			bool AddString(const ConstStringFragment, const ConstStringFragment) throw();

			bool AddValue(const Encode::Value&) throw();
			bool AddValue(const ConstStringFragment, const Encode::Value&) throw();
		};

		static bool SOLAIRE_EXPORT_CALL Read(ReadStream&, Reader&);
		static Encode::Value SOLAIRE_EXPORT_CALL Read(ReadStream&);
		static Encode::Value SOLAIRE_EXPORT_CALL Read(Allocator&, ReadStream&);
	};


}

#endif