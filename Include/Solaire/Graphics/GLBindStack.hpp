#ifndef SOLAIRE_GRAPHICS_GL_BIND_STACK_HPP
#define SOLAIRE_GRAPHICS_GL_BIND_STACK_HPP

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
\file GLBindStack.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 27th October 2015
Last Modified	: 27th October 2015
*/

#include <map>
#include "Graphics.inl"

namespace Solaire{ namespace Graphics{

    class GLBindStack{
    public:
        typedef void(*BindFn)(const GLenum, const void* const);
        typedef void(*UnbindFn)(const GLenum, const void* const);
    private:
        typedef DynamicArray<void*> BindStack;
    private:
        std::map<GLenum, BindStack> mStacks;
        BindFn mBindFn;
        UnbindFn mUnbindFn;
    private:
        DynamicArray<void*>& GetStack(const GLenum aTarget){
            auto it = mStacks.find(aTarget);
            if(it == mStacks.end()) throw std::runtime_error("GLBindStack : Invalid bind target");
            return it->second;
        }

        const DynamicArray<void*>& GetStack(const GLenum aTarget) const{
            auto it = mStacks.find(aTarget);
            if(it == mStacks.end()) throw std::runtime_error("GLBindStack : Invalid bind target");
            return it->second;
        }
    public:
        GLBindStack(Allocator& aAllocator, BindFn aBindFn, UnbindFn aUnbindFn, const std::initializer_list<GLenum> aTargets):
            mBindFn(aBindFn),
            mUnbindFn(aUnbindFn)
        {
            for(const GLenum target : aTargets){
                mStacks.emplace(target, BindStack(aAllocator));
            }
        }

        ~GLBindStack(){

        }

        void Bind(const GLenum aTarget, void* const aObject){
            BindStack& stack = GetStack(aTarget);

            if(stack.IsEmpty() || stack.Back() != aObject){
                stack.PushBack(aObject);
                mBindFn(aTarget, aObject);
            }
        }

        void Unbind(const GLenum aTarget, void* const aObject){
            BindStack& stack = GetStack(aTarget);

            if(stack.IsEmpty()) throw std::runtime_error("GLBindStack : No objects are bound to target");

            if(stack.Back() == aObject){
                stack.PopBack();

                if(stack.IsEmpty()){
                    mBindFn(aTarget, nullptr);
                }else{
                    mBindFn(aTarget, stack.Back());
                }
            }else{
                auto it = stack.FindLast(aObject);
                if(it == stack.end())throw std::runtime_error("GLBindStack : Object was not bound to target");
                stack.Erase(it);
            }

        }
    };
}}

#endif
