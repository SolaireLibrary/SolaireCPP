#ifndef SOLAIRE_GRAPHICS_OBJ_DOC_HPP
#define SOLAIRE_GRAPHICS_OBJ_DOC_HPP

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
\file ObjDoc.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 27th September 2015
Last Modified	: 27th September 2015
*/

#include <istream>
#include <ostream>
#include "..\Core\DataStructures\DynamicArray.hpp"
#include "..\Maths\Vector.hpp"

namespace Solaire{ namespace Graphics{

    namespace Obj{

        typedef Maths::Vector<float, 3> Vertex;
        typedef Maths::Vector<float, 3> Normal;
        typedef Maths::Vector<float, 2> Texture;

        struct FaceElement{
            uint32_t v;
            uint32_t t;
            uint32_t n;
        };

        struct Face{
            FaceElement elements[4];
            uint8_t elementCount;
        };

        class Doc{
        public:
            Allocator& allocator;
            DynamicArray<Face, const Face, uint32_t> faces;
            DynamicArray<Vertex, const Vertex, uint32_t> vertices;
            DynamicArray<Normal, const Normal, uint32_t> normals;
            DynamicArray<Texture, const Texture, uint32_t> textures;

            Doc(Allocator& aAllocator = GetDefaultAllocator()):
                allocator(aAllocator),
                faces(1024, aAllocator),
                vertices(1024, aAllocator),
                normals(1024, aAllocator),
                textures(1024, aAllocator)
            {}
        };

        class Parser{
        public:
            virtual ~Parser(){}

            virtual bool Read(Doc& aDoc, std::istream& aStream) = 0;
            virtual bool Write(const Doc& aDoc, std::ostream& aStream) = 0;
        };
    }

}}

#endif
