#ifndef SOLAIRE_CORE_STRING_FILE_FACTORY_HPP
#define SOLAIRE_CORE_STRING_FILE_FACTORY_HPP

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
	\file StringFileFactory.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 27th September 2015
	Last Modified	: 27th September 2015
*/

#include <iterator>
#include <fstream>
#include "StringBufferFactory.hpp"

namespace Solaire{

    class StringFileFactory : public StringBufferFactory{
    private:
        std::string mFile;
    protected:
        // Inherited from ResourceFactory

        void Lock() override{
            std::ifstream file(mFile);
            if(! file.is_open()) throw std::runtime_error("StringFileFactory : Could not open requested file for reading");

            mBuffer = std::string(
                std::istream_iterator<char>(file),
                std::istream_iterator<char>()
            );

            file.close();
        }

        void Unlock(){
            mBuffer.clear();
        }

    public:
        StringFileFactory(const std::string& aFile) :
            StringBufferFactory(),
            mFile(aFile)
        {}

        StringFileFactory(const StringFileFactory& aOther) :
            StringBufferFactory(aOther),
            mFile(aOther.mFile)
        {}

        StringFileFactory(StringFileFactory&& aOther) :
            StringBufferFactory(std::move(aOther)),
            mFile(std::move(aOther.mFile))
        {}

        ~StringFileFactory(){

        }
    };

}

#endif
