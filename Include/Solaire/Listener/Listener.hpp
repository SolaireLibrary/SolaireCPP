#ifndef SOLAIRE_LISTENER_HPP
#define SOLAIRE_LISTENER_HPP

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
	\file Listener.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 26th September 2015
	Last Modified	: 19th November 2015
*/

#include "Source.hpp"

namespace Solaire{

	class Source;

	class SOLAIRE_EXPORT_API Listener {
	public:
		inline bool SOLAIRE_EXPORT_API Listen(Source& aSource) throw() {
			return aSource.AddListener(*this);
		}

		inline bool SOLAIRE_EXPORT_API Unlisten(Source& aSource) throw() {
			return aSource.RemoveListener(*this);
		}

		inline bool SOLAIRE_EXPORT_API IsListening(const Source& aSource) const throw() {
			return aSource.HasListener(*this);
		}

		virtual bool SOLAIRE_EXPORT_API CanAcceptSource(const Source&) const throw() = 0;
		virtual void SOLAIRE_EXPORT_API OnListen(Source&) throw() = 0;
		virtual void SOLAIRE_EXPORT_API OnUnlisten(const Source&) throw() = 0;
	};

}


#endif
