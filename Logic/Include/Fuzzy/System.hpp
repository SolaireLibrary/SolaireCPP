#ifndef SOLAIRE_LOGIC_FUZZY_SYSTEM_HPP
#define SOLAIRE_LOGIC_FUZZY_SYSTEM_HPP

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
	\file System.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 10th September 2015
	Last Modified	: 10th September 2015
*/

#include <string>
#include "Internals.hpp"
#include "Controller.hpp"
#include "Script.hpp"

namespace Solaire{ namespace Logic{ namespace Fuzzy{

    class System{
    private:
        Controller* mController;
        const Script* mScript;
    public:
        System(){

        }

        ~System(){

        }

        void SetController(Controller& aController){
            mController = &aController;
        }

        void SetRules(const Script& aScript){
            mScript = &aScript;
        }

        void Run(){
            mScript->operator()(*mController);
        }

        void operator()(){
            Run();
        }
    };

}}}


#endif
