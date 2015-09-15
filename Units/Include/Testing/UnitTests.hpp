#ifndef SOLAIRE_UNITS_TEST_HPP
#define SOLAIRE_UNITS_TEST_HPP

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

#include "DigitalTests.hpp"
#include "DistanceTests.hpp"
#include "MassTests.hpp"
#include "MetricTests.hpp"
#include "TemperatureTests.hpp"
#include "VolumeTests.hpp"

namespace Solaire{ namespace Units {

	static void Tests(Test::TestManager& aManager){
		using namespace Testing;
		DigitalTests(aManager);
		DistanceTests(aManager);
		MassTests(aManager);
		MetricTests(aManager);
		TemperatureTests(aManager);
		VolumeTests(aManager);
	}
}}


#endif