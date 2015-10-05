#ifndef SOLAIRE_JSON_SERIALISATION_HPP
#define SOLAIRE_JSON_SERIALISATION_HPP

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
	\file Serialisation.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "Value.hpp"
#include "..\Core\Serialise\Serialisable.hpp"
#include "..\Core\Serialise\SerialisationInterface.hpp"
#include "..\Core\Streams\StreamIterator.hpp"

namespace Solaire{ namespace Json{

    class JsonSerialSystem;
    class JsonSerialObject;

    class JsonSerialArray : public SerialArray{
    private:
        Allocator::SharedPointer<Value> mValue;

        Value& LookupIndex(const SerialIndex aIndex) const{
            return mValue->AsArray()[aIndex];
        }

        template<class T>
        Value& LookupOrCreateIndex(const SerialIndex aIndex, const T aValue){
            Allocator& allocator = mValue->GetAllocator();
            ArrayValue& array_ = mValue->AsArray();
            const SerialIndex size = mValue->AsArray().Size();

            if(aIndex == size){
                return array_.PushBack(allocator.SharedAllocate<Value>(allocator, aValue));
            }else if(aIndex < size){
                return array_[aIndex];
            }else{
                for(SerialIndex i = size; i < aIndex; ++i){
                    array_.PushBack(allocator.SharedAllocate<Value>(allocator, TYPE_NULL));
                }
                return array_.PushBack(allocator.SharedAllocate<Value>(allocator, aValue));
            }
        }

    public:
        friend JsonSerialObject;
        friend JsonSerialSystem;

        JsonSerialArray(Allocator::SharedPointer<Value> aValue) :
            mValue(aValue)
        {}

        ~JsonSerialArray(){

        }

        // Inherited from SerialArray

        SerialIndex Size() const override{
            return mValue->AsArray().Size();
        }

        SerialType TypeOf(const SerialIndex aIndex) const override{
            switch(LookupIndex(aIndex).GetType()){
            case TYPE_NULL:
                return SERIAL_TYPE_N;
            case TYPE_BOOL:
                return SERIAL_TYPE_B;
            case TYPE_NUMBER:
                return SERIAL_TYPE_D;
            case TYPE_STRING:
                return SERIAL_TYPE_S;
            case TYPE_ARRAY:
                return SERIAL_TYPE_A;
            case TYPE_OBJECT:
                return SERIAL_TYPE_O;
            default:
                throw std::runtime_error("Json::SerialObject : Unknown type");
            }
        }
    protected:
        // Inherited from SerialArray

        bool ReadB(const SerialIndex aIndex) const override{
            return LookupIndex(aIndex).AsBool();
        }

        char ReadC(const SerialIndex aIndex) const override{
            return LookupIndex(aIndex).AsString();
        }

        uint8_t ReadU8(const SerialIndex aIndex) const override{
            return static_cast<uint8_t>(LookupIndex(aIndex).AsNumber());
        }

        uint16_t ReadU16(const SerialIndex aIndex) const override{
            return static_cast<uint16_t>(LookupIndex(aIndex).AsNumber());
        }

        uint32_t ReadU32(const SerialIndex aIndex) const override{
            return static_cast<uint32_t>(LookupIndex(aIndex).AsNumber());
        }

        uint64_t ReadU64(const SerialIndex aIndex) const override{
            return static_cast<uint64_t>(LookupIndex(aIndex).AsNumber());
        }

        int8_t ReadI8(const SerialIndex aIndex) const override{
            return static_cast<int8_t>(LookupIndex(aIndex).AsNumber());
        }

        int16_t ReadI16(const SerialIndex aIndex) const override{
            return static_cast<int16_t>(LookupIndex(aIndex).AsNumber());
        }

        int32_t ReadI32(const SerialIndex aIndex) const override{
            return static_cast<int32_t>(LookupIndex(aIndex).AsNumber());
        }

        int64_t ReadI64(const SerialIndex aIndex) const override{
            return static_cast<int64_t>(LookupIndex(aIndex).AsNumber());
        }

        float ReadF(const SerialIndex aIndex) const override{
            return static_cast<float>(LookupIndex(aIndex).AsNumber());
        }

        double ReadD(const SerialIndex aIndex) const override{
            return static_cast<double>(LookupIndex(aIndex).AsNumber());
        }

        SerialString ReadS(const SerialIndex aIndex) const override{
            return LookupIndex(aIndex).AsString();
        }

        SerialArrayPtr ReadA(const SerialIndex aIndex) override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialArray>(LookupIndex(aIndex).AsArray()[aIndex].shared_from_this());
        }

        SerialObjectPtr ReadO(const SerialIndex aIndex) override;

        ConstSerialArrayPtr ReadA(const SerialIndex aIndex) const override{
            Allocator& allocator = mValue->GetAllocator();
            return allocator.SharedAllocate<JsonSerialArray>(LookupIndex(aIndex).AsArray()[aIndex].shared_from_this());
        }

        ConstSerialObjectPtr ReadO(const SerialIndex aIndex) const override;

        void WriteN(const SerialIndex aIndex) override{
            LookupOrCreateIndex(aIndex, TYPE_NULL);
        }

        void WriteB(const SerialIndex aIndex, const bool aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteC(const SerialIndex aIndex, const char aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteU8(const SerialIndex aIndex, const uint8_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteU16(const SerialIndex aIndex, const uint16_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteU32(const SerialIndex aIndex, const uint32_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteU64(const SerialIndex aIndex, const uint64_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteI8(const SerialIndex aIndex, const int8_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteI16(const SerialIndex aIndex, const int16_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteI32(const SerialIndex aIndex, const int32_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteI64(const SerialIndex aIndex, const int64_t aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteF(const SerialIndex aIndex, const float aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteD(const SerialIndex aIndex, const double aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteS(const SerialIndex aIndex, const SerialString aValue) override{
            LookupOrCreateIndex(aIndex, aValue);
        }

        void WriteA(const SerialIndex aIndex, SerialArrayPtr aArray) override{
            LookupOrCreateIndex(aIndex, TYPE_ARRAY) = std::move(*std::static_pointer_cast<JsonSerialArray>(aArray)->mValue);
        }

        void WriteO(const SerialIndex aIndex, SerialObjectPtr aObject) override;
    };

   class JsonSerialObject : public SerialObject{
    private:
        Allocator::SharedPointer<Value> mValue;

        Value& LookupTag(const SerialTag aTag) const{
            return mValue->AsObject()[aTag];
        }

        template<class T>
        Value& LookupOrCreateTag(const SerialTag aTag, const T aValue){
            ObjectValue& object = mValue->AsObject();

            if(object.Contains(aTag)){
                return object[aTag];
            }else{
                Allocator& allocator = mValue->GetAllocator();
                return object.Add(aTag, allocator.SharedAllocate<Value>(allocator, aValue));
            }
        }

    public:
        friend JsonSerialArray;
        friend JsonSerialSystem;

        JsonSerialObject(Allocator::SharedPointer<Value> aValue) :
            mValue(aValue)
        {}

        ~JsonSerialObject(){

        }

        // Inherited from SerialObject

        SerialType TypeOf(const SerialTag aTag) const override{
            switch(LookupTag(aTag).GetType()){
            case TYPE_NULL:
                return SERIAL_TYPE_N;
            case TYPE_BOOL:
                return SERIAL_TYPE_B;
            case TYPE_NUMBER:
                return SERIAL_TYPE_D;
            case TYPE_STRING:
                return SERIAL_TYPE_S;
            case TYPE_ARRAY:
                return SERIAL_TYPE_A;
            case TYPE_OBJECT:
                return SERIAL_TYPE_O;
            default:
                throw std::runtime_error("Json::SerialObject : Unknown type");
            }
        }
    protected:
        // Inherited from SerialArray

        bool ReadB(const SerialTag aTag) const override{
            return LookupTag(aTag).AsBool();
        }

        char ReadC(const SerialTag aTag) const override{
            return LookupTag(aTag).AsString();
        }

        uint8_t ReadU8(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        uint16_t ReadU16(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        uint32_t ReadU32(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        uint64_t ReadU64(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        int8_t ReadI8(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        int16_t ReadI16(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        int32_t ReadI32(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        int64_t ReadI64(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        float ReadF(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        double ReadD(const SerialTag aTag) const override{
            return LookupTag(aTag).AsNumber();
        }

        SerialString ReadS(const SerialTag aTag) const override{
            return LookupTag(aTag).AsString();
        }

        SerialArrayPtr ReadA(const SerialTag aTag) override{
            return mValue->GetAllocator().SharedAllocate<JsonSerialArray>(LookupTag(aTag).shared_from_this());
        }

        SerialObjectPtr ReadO(const SerialTag aTag) override{
            return mValue->GetAllocator().SharedAllocate<JsonSerialObject>(LookupTag(aTag).shared_from_this());
        }

        ConstSerialArrayPtr ReadA(const SerialTag aTag) const override{
            return mValue->GetAllocator().SharedAllocate<JsonSerialArray>(LookupTag(aTag).shared_from_this());
        }

        ConstSerialObjectPtr ReadO(const SerialTag aTag) const override{
            return mValue->GetAllocator().SharedAllocate<JsonSerialObject>(LookupTag(aTag).shared_from_this());
        }

        void WriteN(const SerialTag aTag) override{
            LookupOrCreateTag(aTag, TYPE_NULL);
        }

        void WriteB(const SerialTag aTag, const bool aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteC(const SerialTag aTag, const char aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteU8(const SerialTag aTag, const uint8_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteU16(const SerialTag aTag, const uint16_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteU32(const SerialTag aTag, const uint32_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteU64(const SerialTag aTag, const uint64_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteI8(const SerialTag aTag, const int8_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteI16(const SerialTag aTag, const int16_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteI32(const SerialTag aTag, const int32_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteI64(const SerialTag aTag, const int64_t aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteF(const SerialTag aTag, const float aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteD(const SerialTag aTag, const double aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteS(const SerialTag aTag, const SerialString aValue) override{
            LookupOrCreateTag(aTag, aValue);
        }

        void WriteA(const SerialTag aTag, SerialArrayPtr aArray) override{
            LookupOrCreateTag(aTag, TYPE_ARRAY) = std::move(*std::static_pointer_cast<JsonSerialArray>(aArray)->mValue);
        }

        void WriteO(const SerialTag aTag, SerialObjectPtr aObject) override{
            LookupOrCreateTag(aTag, TYPE_OBJECT) = std::move(*std::static_pointer_cast<JsonSerialObject>(aObject)->mValue);
        }
    };

    SerialObjectPtr JsonSerialArray::ReadO(const SerialIndex aIndex){
        Allocator& allocator = mValue->GetAllocator();
        return allocator.SharedAllocate<JsonSerialObject>(LookupIndex(aIndex).AsArray()[aIndex].shared_from_this());
    }

    ConstSerialObjectPtr JsonSerialArray::ReadO(const SerialIndex aIndex) const{
        Allocator& allocator = mValue->GetAllocator();
        return allocator.SharedAllocate<JsonSerialObject>(LookupIndex(aIndex).AsArray()[aIndex].shared_from_this());
    }

    void JsonSerialArray::WriteO(const SerialIndex aIndex, SerialObjectPtr aObject){
        LookupOrCreateIndex(aIndex, TYPE_OBJECT) = std::move(*std::static_pointer_cast<JsonSerialObject>(aObject)->mValue);
    }

    class JsonSerialSystem : public SerialSystem{
    private:
        Allocator& mParseAllocator;
        Allocator& mDataAllocator;

        void WriteValue(std::ostream& aStream, const std::shared_ptr<Value> aElement) const{
            const size_t size = Value::EstimateSerialLength(*aElement);
            String string(mParseAllocator);
            for(size_t i = 0; i < size; ++i) string.PushBack('\0');
            String::Iterator it = string.begin();
            Value::Serialise(it, string.end(), it, *aElement);
            aStream << string;
        }

        std::shared_ptr<Value> ReadValue(std::istream& aStream) const{
            IStreamIterator begin(aStream);
            IStreamIterator end;
            return Value::Deserialise<IStreamIterator>(begin, end, begin, mParseAllocator, mDataAllocator);
        }
    public:
        JsonSerialSystem(Allocator& aParseAllocator, Allocator& aDataAllocator) :
            mParseAllocator(aParseAllocator),
            mDataAllocator(aDataAllocator)
        {}

        ~JsonSerialSystem(){

        }

        // Inherited from SerialSystem

        SerialArrayPtr CreateA() const override{
            return mParseAllocator.SharedAllocate<JsonSerialArray>(mParseAllocator.SharedAllocate<Value>(mParseAllocator, TYPE_ARRAY));
        }

        SerialObjectPtr CreateO() const override{
            return mParseAllocator.SharedAllocate<JsonSerialObject>(mParseAllocator.SharedAllocate<Value>(mParseAllocator, TYPE_OBJECT));
        }

        void WriteA(std::ostream& aStream, const ConstSerialArrayPtr aArray) const override{
            WriteValue(aStream, std::static_pointer_cast<const JsonSerialArray>(aArray)->mValue);
        }

        void WriteO(std::ostream& aStream, const ConstSerialObjectPtr aObject) const override{
            WriteValue(aStream, std::static_pointer_cast<const JsonSerialObject>(aObject)->mValue);
        }

        SerialArrayPtr ReadA(std::istream& aStream) const override{
             return GetParseAllocator().SharedAllocate<JsonSerialArray>(ReadValue(aStream));
        }

        SerialObjectPtr ReadO(std::istream& aStream) const override{
            return GetParseAllocator().SharedAllocate<JsonSerialObject>(ReadValue(aStream));
        }

        Allocator& GetParseAllocator() const override{
            return mParseAllocator;
        };

        Allocator& GetDataAllocator() const override{
            return mDataAllocator;
        };

    };

    template<class T>
    std::shared_ptr<Value> JsonSerialise(Allocator& aParseAllocator, Allocator& aDataAllocator, T aValue){
        JsonSerialSystem system(aParseAllocator, aDataAllocator);
        std::shared_ptr<JsonSerialObject> object = system.CreateO();

        Serialisable<T>::Serialise(aValue, system, "jsonSerialise", object);
        return object->mValue->AsObject()["jsonSerialise"];
    }

    template<class T>
    typename Serialisable<T>::DeserialiseType JsonDeserialise(Allocator& aParseAllocator, Allocator& aDataAllocator, std::shared_ptr<Value> aValue){
        JsonSerialSystem system(aParseAllocator, aDataAllocator);
        std::shared_ptr<JsonSerialObject> object = system.CreateO();

        return object->mValue->AsObject().Add("jsonDeserialise", aValue);
        return Serialisable<T>::Deserialise(object, "jsonDeserialise", aValue);
    }

    template<class ...Params>
    std::shared_ptr<Value> JsonSerialiseMany(Allocator& aParseAllocator, Allocator& aDataAllocator,  Params&& ...aParams){
        std::shared_ptr<Value> value = aDataAllocator.SharedAllocate<Value>(aDataAllocator, TYPE_ARRAY);
        ArrayValue& array_ = value->AsArray();
        array_.PushBack(JsonSerialise(aParseAllocator, aDataAllocator, aParams...));
        return value;
    }
}}


#endif
