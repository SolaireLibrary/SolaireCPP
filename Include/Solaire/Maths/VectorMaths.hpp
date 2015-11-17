#ifndef SOLAIRE_VECTOR_MATHS_HPP
#define SOLAIRE_VECTOR_MATHS_HPP

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
	\file VectorMaths.hpp
	\brief Contains code for 4 dimentional vector maths.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.1
	\date
	Created			: 23rd September 2015
	Last Modified	: 11th November 2015
*/

#include <cmath>
#include <cstdint>
#include "..\Core\Init.hpp"

namespace Solaire{

	template<class T, const uint32_t LENGTH>
	class VectorMaths{
	public:
		typedef T Scalar;
		typedef typename TypeTraits<Scalar>::ConstPassType ScalarPass;
	public:
		static T* SOLAIRE_EXPORT_CALL Copy(T* const aFirst, const T* const aSecond) throw() {
			std::memcpy(aFirst, aSecond, sizeof(T) * LENGTH);
			return aFirst;
		}

		static bool SOLAIRE_EXPORT_CALL Equals(const T* const aFirst, const T* const aSecond) throw() {
			return std::memcmp(aFirst, aSecond, LENGTH * sizeof(T)) == 0;
		}

		static bool SOLAIRE_EXPORT_CALL NotEquals(const T* const aFirst, const T* const aSecond) throw() {
			return std::memcmp(aFirst, aSecond, LENGTH * sizeof(T)) != 0;
		}

		////

		static T* SOLAIRE_EXPORT_CALL AddEq(T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] += aSecond[i];
			}
			return aFirst;
		}

		static T* SOLAIRE_EXPORT_CALL SubEq(T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] -= aSecond[i];
			}
			return aFirst;
		}

		static T* SOLAIRE_EXPORT_CALL MulEq(T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] *= aSecond[i];
			}
			return aFirst;
		}

		static T* SOLAIRE_EXPORT_CALL DivEq(T* const aVector, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] /= aSecond[i];
			}
			return aVector;
		}

		static T* SOLAIRE_EXPORT_CALL AddEq(T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] += aScalar;
			}
			return aVector;
		}

		static T* SOLAIRE_EXPORT_CALL SubEq(T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] -= aScalar;
			}
			return aVector;
		}

		static T* SOLAIRE_EXPORT_CALL MulEq(T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] *= aScalar;
			}
			return aVector;
		}

		static T* SOLAIRE_EXPORT_CALL DivEq(T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] /= aScalar;
			}
			return aFirst;
		}

		////

		static T* SOLAIRE_EXPORT_CALL Add(T* const aOutput, const T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] + aSecond[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Sub(T* const aOutput, const T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] - aSecond[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Mul(T* const aOutput, const T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] * aSecond[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Div(T* const aOutput, const T* const aFirst, const T* const aSecond) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aFirst[i] / aSecond[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Add(T* const aOutput, const T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] + aScalar;
			}
			return aOutput;
		}

		static T*  SOLAIRE_EXPORT_CALL Sub(T* const aOutput, const T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] - aScalar;
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Mul(T* const aOutput, const T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] * aScalar;
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Div(T* const aOutput, const T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[i] / aScalar;
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Add(T* const aOutput, const ScalarPass aScalar, const T* const aVector) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar + aVector[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Sub(T* const aOutput, const ScalarPass aScalar, const T* const aVector) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar - aVector[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Mul(T* const aOutput, const ScalarPass aScalar, const T* const aVector) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar * aVector[i];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL Div(T* const aOutput, const ScalarPass aScalar, const T* const aVector) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aScalar / aVector[i];
			}
			return aOutput;
		}

		////

        static T* SOLAIRE_EXPORT_CALL LerpEq(T* const aFirst, const T* const aSecond, const double aWeight) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aFirst[i] = static_cast<T>((1.0 - aWeight) * static_cast<double>(aFirst[i]) + aWeight * static_cast<double>(aSecond[i]));
			}
			return aFirst;
		}

		static T* SOLAIRE_EXPORT_CALL Lerp(T* const aOutput, const T* const aFirst, const T* const aSecond, const double aWeight) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = static_cast<T>((1.0 - aWeight) * static_cast<double>(aFirst[i]) + aWeight * static_cast<double>(aSecond[i]));
			}
			return aOutput;
		}

		static T SOLAIRE_EXPORT_CALL Sum(const T* const aVector) throw() {
			T sum = static_cast<T>(0);
			for(uint32_t i = 0; i < LENGTH; ++i) sum += aVector[i];
			return sum;
		}

		static T SOLAIRE_EXPORT_CALL Avg(const T* const aVector) throw() {
			return Sum(aVector) / static_cast<T>(LENGTH);
		}

		static T SOLAIRE_EXPORT_CALL MagSq(const T* aVector) throw() {
			T tmp = static_cast<T>(0);
			for (uint32_t i = 0; i < LENGTH; ++i){
				sum += aVector[i] * aVector[i];
			}
			return tmp;
		}

		static double SOLAIRE_EXPORT_CALL Mag(const T* const aVector) throw() {
			return std::sqrt(static_cast<double>(MagSq(aVector)));
		}

		static T SOLAIRE_EXPORT_CALL Dot(const T* const aFirst, const T* const aSecond) throw() {
			T tmp = static_cast<T>(0);
			for (uint32_t i = 0; i < LENGTH; ++i) {
				tmp += aFirst[i] * aSecond[i];
			}
			return tmp;
		}

		template<const uint32_t L = LENGTH, typename ENABLE = typename std::enable_if<LENGTH == 3>::type>
		static T* SOLAIRE_EXPORT_CALL Cross(T* const aOutput, const T* const aFirst, const T* const aSecond) throw() {
			aOutput[0] = (aFirst[1] * aSecond[2]) - (aFirst[2] * aSecond[1]);
			aOutput[1] = (aFirst[2] * aSecond[0]) - (aFirst[0] * aSecond[2]);
			aOutput[2] = (aFirst[0] * aSecond[1]) - (aFirst[1] * aSecond[0]);
			return aOutput;
		}
		
		static T* SOLAIRE_EXPORT_CALL Swizzle(T* const aOutput, const T* const aVector, const uint32_t* const aSwizzle)  throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = aVector[aSwizzle[i]];
			}
			return aOutput;
		}

		static T* SOLAIRE_EXPORT_CALL SwizzleEq(T* const aVector, const uint32_t* const aSwizzle) throw() {
			T tmp[LENGTH];
			return Swizzle(aVector, Copy(tmp, aVector), aSwizzle);
		}

		static T* SOLAIRE_EXPORT_CALL NormaliseEq(T* const aVector) throw() {
			return DivEq(aVector, Mag(aVector));
		}

		static T* SOLAIRE_EXPORT_CALL Normalise(T* const aOutput, const T* const aVector) throw() {
			return NormaliseEq(Copy(aOutput, aVector));
		}

		static T* SOLAIRE_EXPORT_CALL Fill(T* const aVector, const ScalarPass aScalar) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aVector[i] = aScalar;
			}
			return aVector;
		} 
		
		template<class T2>
		static T* SOLAIRE_EXPORT_CALL Cast(T* const aOutput, const T2* const aVector) throw() {
			for(uint32_t i = 0; i < LENGTH; ++i) {
				aOutput[i] = static_cast<T>(aSecond[i]);
			}

			return aVector;
		}
	};
}


#endif
