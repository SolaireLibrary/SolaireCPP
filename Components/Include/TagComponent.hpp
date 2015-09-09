#ifndef SOLAIRE_COMPONENTS_TAG_COMPONENT_HPP
#define SOLAIRE_COMPONENTS_TAG_COMPONENT_HPP

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
	\file TagComponent.hpp
	\brief Contains the code for the TagComponent class.
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date 
	Created			: 7th September 2015
	Last Modified	: 7th September 2015
*/

#include <string>
#include "Composite.hpp"

namespace Solaire{ namespace Components{

	class TagComponent : public Component
	{
	private:
		TagComponent(Component&&);
		TagComponent& operator=(Component&&);

		std::vector<const std::string> mTags;
	protected:
		// Inherited from Component

		bool PreAttach(Composite& aNewParent) const override{
			return aNewParent.GetComponent<TagComponent>() == aNewParent.ComponentEnd();
		}

		void PostAttach(){

		}

		bool PreDetach() const{
			return true;
		}

		void PostDetach(Composite& aOldParent){
			
		}

	public:
		template<class CALLBACK, class tag_iterator>
		static size_t ForEachComposite(CALLBACK aCallback, tag_iterator aBegin, tag_iterator aEnd){
			auto condition = [&](Component& aComponent){
				TagComponent* tag = dynamic_cast<TagComponent*>(&aComponent);
				if(tag == nullptr) return false;
				return tag->HasTags(aBegin, aEnd);
			};

			auto action = [&](Component& aComponent){
				aCallback(aComponent.GetParent());
			};

			return Composite::ForEachComponent(condition, action);
		}

		template<class CALLBACK>
		static size_t ForEachComposite(CALLBACK aCallback, const std::initializer_list<const std::string> aTags){
			return ForEachComposite(aCallback, aTags.begin(), aTags.end());
		}

		template<class CALLBACK>
		static size_t ForEachComposite(CALLBACK aCallback, const std::string& aTag){
			return TagComponent::ForEachComposite({aTag}, aCallback);
		}

		typedef std::vector<const std::string>::const_iterator tag_iterator;

		TagComponent(){

		}

		TagComponent(const std::string& aTag){
			AddTag(aTag);
		}

		TagComponent(const std::initializer_list<const std::string> aTags){
			AddTags(aTags.begin(), aTags.end());
		}

		~TagComponent(){

		}

		TagComponent(const TagComponent& aOther) :
			mTags(aOther.mTags)
		{

		}

		TagComponent& operator=(const TagComponent& aOther){
			mTags = aOther.mTags;
			return *this;
		}

		tag_iterator TagBegin() const{
			return mTags.begin();
		}

		tag_iterator TagEnd() const{
			return mTags.end();
		}

		size_t TagCount() const{
			return mTags.size();
		}

		const std::string& GetTag(size_t aIndex = 0) const{
			return mTags[aIndex];
		}

		bool AddTag(const std::string& aTag){
			if(HasTag(aTag)) return false;
			mTags.push_back(aTag);
			return true;
		}

		template<class tag_iterator>
		bool AddTags(tag_iterator aBegin, tag_iterator aEnd){
			bool addedAll = true;
			for(aBegin; aBegin != aEnd; ++aBegin){
				addedAll = addedAll && AddTag(*aBegin);
			}
			return addedAll;
		}

		bool AddTags(const std::initializer_list<const std::string> aTags){
			return AddTags(aTags.begin(), aTags.end());
		}

		bool RemoveTag(const std::string& aTag){
			auto it = std::find(mTags.begin(), mTags.end(), aTag);
			if(it == mTags.end()) return false;
			mTags.erase(it);
			return true;
		}

		template<class tag_iterator>
		bool RemoveTags(tag_iterator aBegin, tag_iterator aEnd){
			bool removedAll = true;
			for(aBegin; aBegin != aEnd; ++aBegin){
				removedAll = removedAll && RemoveTag(*aBegin);
			}
			return removedAll;
		}

		bool RemoveTags(const std::initializer_list<const std::string> aTags){
			return RemoveTags(aTags.begin(), aTags.end());
		}

		bool HasTag(const std::string& aTag) const{
			return std::find(mTags.begin(), mTags.end(), aTag) != mTags.end();
		}

		template<class tag_iterator>
		bool HasTags(tag_iterator aBegin, tag_iterator aEnd) const{
			for(aBegin; aBegin != aEnd; ++aBegin){
				if(! HasTag(*aBegin)) return false;
			}
			return true;
		}

		bool HasTags(const std::initializer_list<const std::string> aTags) const{
			return HasTags(aTags.begin(), aTags.end());
		}

		bool operator==(const TagComponent& aOther) const{
			return aOther.HasTags(mTags.begin(), mTags.end());
		}

		bool operator!=(const TagComponent& aOther) const{
			return ! aOther.HasTags(mTags.begin(), mTags.end());
		}

		bool operator==(const std::initializer_list<const std::string> aTags) const{
			return HasTags(aTags);
		}

		bool operator!=(const std::initializer_list<const std::string> aTags) const{
			return ! HasTags(aTags);
		}
	};
}}


#endif