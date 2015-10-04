#ifndef SOLAIRE_CORE_STRING_FACTORY_HPP
#define SOLAIRE_CORE_STRING_FACTORY_HPP

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
	\file StringFactory.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 27th September 2015
	Last Modified	: 27th September 2015
*/

#include <string>
#include "ResourceFactory.hpp"

namespace Solaire{

    class StringFactory : public ResourceFactory{
    private:
    public:
        StringFactory() :
            ResourceFactory()
        {}

        StringFactory(const StringFactory& aOther) :
            ResourceFactory(aOther)
        {}

        StringFactory(StringFactory&& aOther) :
            ResourceFactory(std::move(aOther))
        {}

        virtual ~StringFactory(){

        }

        virtual const std::string& GetString() const = 0;
    };

}

#endif
