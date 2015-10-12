#ifndef SOLAIRE_JSON_RPC_REQUEST_HPP
#define SOLAIRE_JSON_RPC_REQUEST_HPP

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
\file RpcRequest.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 2.0
\date
Created			: 4th October 2015
Last Modified	: 5th October 2015
*/

#include "Value.hpp"
#include "..\Serialise\Serialisable.hpp"
#include "..\Serialise\PrimativeSerialisables.hpp"

namespace Solaire{ namespace Json{

    typedef int64_t RpcRequestID;

    class RpcRequest{
    private:
        String mMethodName;
        std::shared_ptr<Value> mParams;
        RpcRequestID mID;
    protected:
        enum : RpcRequestID{
            RPC_NOTIFICATION_ID = INT64_MAX
        };
    public:
        static std::shared_ptr<Value> Serialise(const RpcRequest& aRequest);
    public:
        RpcRequest(Allocator& aAllocator);
        ~RpcRequest();

        void SetMethodName(const ConstStringFragment aName);
        ConstStringFragment GetMethodName() const;

        bool IsRequest() const;
        bool IsNotification() const;

        void SetNotification();
        void SetRequestID(const RpcRequestID aID);
        RpcRequestID GetRequestID() const;

        void SetParams(std::shared_ptr<Value> aParams);
        std::shared_ptr<const Value> GetParams() const;

        Allocator& GetAllocator() const;
    };

}}

#include "RpcRequest.inl"

#endif