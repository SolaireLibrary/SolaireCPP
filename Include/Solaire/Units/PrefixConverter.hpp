
#ifndef SOLAIRE_UNITS_PREFIX_CONVERTER_HPP
#define SOLAIRE_UNITS_PREFIX_CONVERTER_HPP

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

#include "BaseConverter.hpp"

namespace Solaire{
	namespace Units{

#define SOLAIRE_UNITS_PREFIXED_CONVERTER_COMMON(class_name, prefix_converter)\
\
	static constexpr conversion_t Convert(const typename prefix_converter::unit_t aInputPrefix, const unit_t aInputUnit, const typename prefix_converter::unit_t aOutputPrefix, const unit_t aOutputUnit, const conversion_t aValue){\
        return\
            prefix_converter::Convert(\
                prefix_converter::unit_t::NONE,\
                aOutputPrefix,\
                class_name::Convert(\
                    aInputUnit,\
                    aOutputUnit,\
                    prefix_converter::Convert(\
                        aInputPrefix,\
                        prefix_converter::unit_t::NONE,\
                        aValue\
                    )\
                )\
            );\
    }\
\
	constexpr conversion_t Get(const prefix_t aPrefix, const unit_t aUnit) const{\
		return prefix_converter::Convert(prefix_converter::INTERMEDIARY_UNIT, aPrefix, Get(aUnit));\
	}\
	void Set(const prefix_t aPrefix, unit_t aUnit, const conversion_t aValue){\
		Set(aUnit, prefix_converter::Convert(aPrefix, prefix_converter::INTERMEDIARY_UNIT, aValue));\
	}

#ifndef SOLAIRE_UNITS_NO_PROPERTIES
	template<class CONVERTER, typename CONVERTER::prefix_t PREFIX, typename CONVERTER::unit_t UNIT>
	class PrefixConverterProperty{
	private:
		CONVERTER* mParent;
	public:
		void operator=(const typename CONVERTER::conversion_t aValue){
			return mParent->Set(PREFIX, UNIT, aValue);
		}

		constexpr operator typename CONVERTER::conversion_t() const{
			return mParent->Get(PREFIX, UNIT);
		}

		void operator+=(const typename CONVERTER::conversion_t aValue){
			mParent->Set(PREFIX, UNIT, mParent->PrefixedGet(PREFIX, UNIT) + aValue);
		}

		void operator-=(const typename CONVERTER::conversion_t aValue){
			mParent->Set(PREFIX, UNIT, mParent->PrefixedGetPREFIX, (UNIT) - aValue);
		}

		void operator*=(const typename CONVERTER::conversion_t aValue){
			mParent->Set(PREFIX, UNIT, mParent->PrefixedGetPREFIX, (UNIT) * aValue);
		}

		void operator/=(const typename CONVERTER::conversion_t aValue){
			mParent->Set(PREFIX, UNIT, mParent->PrefixedGet(PREFIX, UNIT) / aValue);
		}
	};
#endif
}}


#endif