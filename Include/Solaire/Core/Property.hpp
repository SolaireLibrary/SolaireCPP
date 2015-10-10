#ifndef SOLAIRE_PROPERTY_HPP
#define SOLAIRE_PROPERTY_HPP

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
	\file Property.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 6th October 2015
	Last Modified	: 6th October 2015
*/

#include "Init.hpp"

namespace Solaire{

    enum{
        PROPERTY_READ = 0x1,
        PROPERTY_WRITE = 0x2,
        PROPERTY_READ_WRITE = PROPERTY_READ | PROPERTY_WRITE
    };

    // Pointer to member

    template<class Parent, class Type, Type Parent::* const Member, const int PassMode, const int ConstMode>
    class MemberProperty
    {
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        MemberProperty(const MemberProperty&) = delete;
        MemberProperty(MemberProperty&&) = delete;
        MemberProperty& operator=(const MemberProperty&) = delete;
        MemberProperty& operator=(MemberProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr MemberProperty(Parent& aParent):
            mParent(aParent)
        {}

        // Write

        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_WRITE, Return>::type
        operator*(){
            return mParent.*Member;
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_WRITE, Type*>::type
        operator->(){
            return &(mParent.*Member);
        }

        template<const int Mode = ConstMode, typename = typename std::enable_if<Mode & PROPERTY_WRITE>::type>
        SOLAIRE_INLINE operator Return(){
            return mParent.*Member;
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_WRITE, Type&>::type
        operator=(ConstReturn aValue){
            return mParent.*Member = aValue;
        }

        // Read

        template<const int Mode = ConstMode, typename = typename std::enable_if<Mode & PROPERTY_READ>::type>
        SOLAIRE_INLINE operator ConstReturn() const{
           return mParent.*Member;
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_READ, ConstReturn>::type
        operator*() const{
            return mParent.*Member;
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_READ, const Type*>::type
        operator->() const{
            return &(mParent.*Member);
        }
    };

    template<class Parent, class Type, Type* Parent::* const Member, const int PassMode, const int ConstMode>
    class DrefMemberProperty
    {
    public:
        typedef typename PassTypes<Type, PassMode>::Type Return;
        typedef typename PassTypes<Type, PassMode>::ConstType ConstReturn;
    private:
        DrefMemberProperty(const DrefMemberProperty&) = delete;
        DrefMemberProperty(DrefMemberProperty&&) = delete;
        DrefMemberProperty& operator=(const DrefMemberProperty&) = delete;
        DrefMemberProperty& operator=(DrefMemberProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr DrefMemberProperty(Parent& aParent):
            mParent(aParent)
        {}

        // Write

        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_WRITE, Return>::type
        operator*(){
            return *(mParent.*Member);
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_WRITE, Type*>::type
        operator->(){
            return mParent.*Member;
        }

        template<const int Mode = ConstMode, typename = typename std::enable_if<Mode & PROPERTY_WRITE>::type>
        SOLAIRE_INLINE operator Return(){
            return (mParent.*Member);
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_WRITE, Type&>::type
        operator=(ConstReturn aValue){
            return *(mParent.*Member) = aValue;
        }

        // Read

        template<const int Mode = ConstMode, typename = typename std::enable_if<Mode & PROPERTY_READ>::type>
        SOLAIRE_INLINE operator ConstReturn() const{
            return *(mParent.*Member);
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_READ, ConstReturn>::type
        operator*() const{
            return *(mParent.*Member);
        }


        template<const int Mode = ConstMode>
        SOLAIRE_INLINE typename std::enable_if<Mode & PROPERTY_READ, const Type*>::type
        operator->() const{
            return mParent.*Member;
        }
    };

    // Pointer to local function

    template<class Parent, class Type, const Type&(Parent::*ConstGet)() const, Type&(Parent::*Get)()>
    class RefFunctionProperty
    {
    private:
        RefFunctionProperty(const RefFunctionProperty&) = delete;
        RefFunctionProperty(RefFunctionProperty&&) = delete;
        RefFunctionProperty& operator=(const RefFunctionProperty&) = delete;
        RefFunctionProperty& operator=(RefFunctionProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr RefFunctionProperty(Parent& aParent):
            mParent(aParent)
        {}

        // Write

        template<Type&(Parent::*Fn)() = Get>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type&>::type
        operator*(){
            return mParent.*Get();
        }


        template<Type&(Parent::*Fn)() = Get>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type*>::type
        operator->(){
            return &mParent.*Get();
        }

        template<Type&(Parent::*Fn)() = Get, typename = typename std::enable_if<Fn != nullptr>::type>
        SOLAIRE_INLINE operator Type&(){
            return mParent.*Get();
        }


        template<Type&(Parent::*Fn)() = Get>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type&>::type
        operator=(const Type& aValue){
            return mParent.*Get() = aValue;
        }

        // Read

        template<const Type&(Parent::*Fn)() const = ConstGet, typename = typename std::enable_if<Fn != nullptr>::type>
        SOLAIRE_INLINE operator const Type&() const{
           return mParent.*ConstGet();
        }


        template<const Type&(Parent::*Fn)() const = ConstGet>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, const Type&>::type
        operator*() const{
            return mParent.*ConstGet();
        }


        template<const Type&(Parent::*Fn)() const = ConstGet>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, const Type*>::type
        operator->() const{
            return &mParent.*ConstGet();
        }
    };

    template<class Parent, class Type, Type(Parent::*ConstGet)() const, Type&(Parent::*Get)()>
    class ValFunctionProperty
    {
    private:
        ValFunctionProperty(const ValFunctionProperty&) = delete;
        ValFunctionProperty(ValFunctionProperty&&) = delete;
        ValFunctionProperty& operator=(const ValFunctionProperty&) = delete;
        ValFunctionProperty& operator=(ValFunctionProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        constexpr ValFunctionProperty(Parent& aParent):
            mParent(aParent)
        {}

        // Write

        template<Type&(Parent::*Fn)() = Get>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type&>::type
        operator*(){
            return mParent.*Get();
        }


        template<Type&(Parent::*Fn)() = Get>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type*>::type
        operator->(){
            return &mParent.*Get();
        }

        template<Type&(Parent::*Fn)() = Get, typename = typename std::enable_if<Fn != nullptr>::type>
        SOLAIRE_INLINE operator Type&(){
            return mParent.*Get();
        }


        template<Type&(Parent::*Fn)() = Get>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type&>::type
        operator=(const Type& aValue){
            return mParent.*Get() = aValue;
        }

        // Read

        template<Type(Parent::*Fn)() const = ConstGet, typename = typename std::enable_if<Fn != nullptr>::type>
        SOLAIRE_INLINE operator Type() const{
           return mParent.*ConstGet();
        }


        template<Type(Parent::*Fn)() const = ConstGet>
        SOLAIRE_INLINE typename std::enable_if<Fn != nullptr, Type>::type
        operator*() const{
            return mParent.*ConstGet();
        }
    };
}


#endif
