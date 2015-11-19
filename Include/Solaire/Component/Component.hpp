#ifndef SOLAIRE_COMPONENT_COMPONENT_HPP
#define SOLAIRE_COMPONENT_COMPONENT_HPP

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
	Created			: 19th November 2015
	Last Modified	: 19th November 2015
*/

#include "Composite.hpp"

namespace Solaire{

	class SOLAIRE_EXPORT_API Component : public Link::Object {
	public:
		virtual Composite& SOLAIRE_EXPORT_CALL GetComposite() const = 0;
		virtual bool SOLAIRE_EXPORT_CALL HasComposite() const = 0;

		virtual ComponentID SOLAIRE_EXPORT_CALL GetClassID() const = 0;

		virtual bool SOLAIRE_EXPORT_CALL CanAttachTo(const Composite&) const = 0;
		virtual bool SOLAIRE_EXPORT_CALL CanDetachFrom(const Composite&) const = 0;

		virtual void SOLAIRE_EXPORT_CALL OnAttachment(Composite&) = 0;
		virtual void SOLAIRE_EXPORT_CALL OnDetachment(Composite&) = 0;

		inline bool AttachTo(Composite& aComposite) {
			return aComposite.Attach(*this);
		}

		inline bool DetachFrom(Composite& aComposite) {
			return aComposite.Detach(*this);
		}
	};

}


#endif
