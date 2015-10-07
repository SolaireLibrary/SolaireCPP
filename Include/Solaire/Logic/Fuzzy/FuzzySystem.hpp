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
\file FuzzySystem.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 7th October 2015
Last Modified	: 7th October 2015
*/

#include "Fuzzifier.hpp"
#include "Defuzzifier.hpp"

namespace Solaire {namespace Fuzzy{

    typedef std::map<String, float> FInputMap;
    typedef std::map<String, DynamicArray<float>> FOutputBuffer;
    typedef std::function<float(const DynamicArray<float>&)> FOutputCombiner;

    class FBody{
    public:
        virtual ~FBody(){}

        virtual float Execute(const FInputMap& aInputs) const = 0;
    };

    class FMembership{
    public:
        virtual ~FMembership(){}

        virtual float CalculateMembership(float aInput) const = 0;
    };

    class FInput : public FBody{
    private:
        String mName;
    public:
        FInput(String aName):
            mName(aName)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            auto it = aInputs.find(mName);
            return it == aInputs.end() ? 0.f : it->second;
        }
    };

    class FUnary : public FBody{
    protected:
        std::shared_ptr<const FBody> mChild;
    public:
        FUnary(std::shared_ptr<const FBody> aChild):
            mChild(aChild)
        {}

        virtual ~FUnary(){

        }
    };

    class FBinary : public FBody{
    protected:
        std::shared_ptr<const FBody> mLeft;
        std::shared_ptr<const FBody> mRight;
    public:
        FBinary(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            mLeft(aLeft),
            mRight(aRight)
        {}

        virtual ~FBinary(){

        }
    };

    class FAdd : public FBinary{
    public:
        FAdd(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return mLeft->Execute(aInputs) + mRight->Execute(aInputs);
        }
    };

    class FSub : public FBinary{
    public:
        FSub(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return mLeft->Execute(aInputs) - mRight->Execute(aInputs);
        }
    };

    class FMul : public FBinary{
    public:
        FMul(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return mLeft->Execute(aInputs) * mRight->Execute(aInputs);
        }
    };

    class FDiv : public FBinary{
    public:
        FDiv(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return mLeft->Execute(aInputs) / mRight->Execute(aInputs);
        }
    };

    class FAnd : public FBinary{
    public:
        FAnd(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return std::min<float>(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }
    };

    class FOr : public FBinary{
    public:
        FOr(std::shared_ptr<const FBody> aLeft, std::shared_ptr<const FBody> aRight):
            FBinary(aLeft, aRight)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return std::max<float>(mLeft->Execute(aInputs), mRight->Execute(aInputs));
        }
    };

    class FNot : public FUnary{
    public:
        FNot(std::shared_ptr<const FBody> aChild):
            FUnary(aChild)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return 1.f - mChild->Execute(aInputs);
        }
    };

    class FBracket : public FUnary{
    public:
        FBracket(std::shared_ptr<const FBody> aChild):
            FUnary(aChild)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return mChild->Execute(aInputs);
        }
    };

    class FMembershipBody : public FUnary{
    private:
        std::shared_ptr<const FMembership> mMembership;
    public:
        FMembershipBody(std::shared_ptr<const FMembership> aMembership, std::shared_ptr<const FBody> aChild):
            FUnary(aChild),
            mMembership(aMembership)
        {}

        // Inherited from FBody
        float Execute(const FInputMap& aInputs) const override{
            return mMembership->CalculateMembership(mChild->Execute(aInputs));
        }
    };

    class FScript{
    private:
        typedef std::pair<String, std::shared_ptr<const FBody>> FLine;
        DynamicArray<FLine> mLines;

        FInputMap mInputMap;
        FOutputBuffer mOutputBuffer;

        std::shared_ptr<const Fuzzifier> mFuzzifier;
        std::shared_ptr<Defuzzifier> mDefuzzifier;
        FOutputCombiner mOutputCombiner;

        Allocator* mAllocator;
    public:
        FScript(Allocator& aAllocator):
            mAllocator(& aAllocator)
        {}

        void SetFuzzifier(std::shared_ptr<const Fuzzifier> aFuzzifier){
            mFuzzifier.swap(aFuzzifier);

            mInputMap.clear();

            const size_t count = mFuzzifier->GetInputCount();
            for(size_t i = 0; i < count; ++i){
                const ConstStringFragment name = mFuzzifier->GetInputName(i);
                mInputMap.emplace(String(name, *mAllocator), 0.f);
            }
        }

        void SetDefuzzifier(std::shared_ptr<Defuzzifier> aDefuzzifier){
            mDefuzzifier.swap(aDefuzzifier);
        }

        void SetDefuzzifier(FOutputCombiner aOutputCombiner){
            mOutputCombiner = aOutputCombiner;
        }

        void Execute(){
            for(std::pair<const String, float>& input : mInputMap){
                input.second = mFuzzifier->GetInputValue(input.first);
            }

            for(const FLine& line : mLines){
                const float output = line.second->Execute(mInputMap);
                DynamicArray<float>& buffer = mOutputBuffer.find(line.first)->second;
                buffer.PushBack(output);
            }

            for(std::pair<const String, DynamicArray<float>>& buffer : mOutputBuffer){
                if(buffer.second.IsEmpty()) buffer.second.PushBack(0.f);
                mDefuzzifier->SetOutputValue(buffer.first, mOutputCombiner(buffer.second));
                buffer.second.Clear();
            }
        }
    };

}}

#endif
