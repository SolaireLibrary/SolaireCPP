#ifndef SOLAIRE_TASK_IMPLEMENTATION_HPP
#define SOLAIRE_TASK_IMPLEMENTATION_HPP
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

#include "Solaire\Threading\TaskI.hpp"
#include "Solaire\Threading\TaskCallbacks.hpp"

#include <mutex>
#include <condition_variable>
#include <memory>

namespace Solaire {

	class TaskImplementation : public TaskCallbacks {
	private:
		std::mutex mLock;
		std::condition_variable mWaitCondition;
		TaskI& mTask;
	public:
		TaskImplementation(TaskI& aTask) throw() :
			mTask(aTask)
		{}

		SOLAIRE_EXPORT_CALL ~TaskImplementation() throw() {

		}

		inline bool Initialise() throw() {
			return mTask.InitialiseI(*this);
		}

		inline bool PreExecute() throw() {
			return mTask.OnPreExecuteI();
		}

		inline bool Execute() throw() {
			return mTask.OnExecuteI();
		}

		inline bool Resume() throw() {
			return mTask.OnResumeI();
		}

		inline bool PostExecute() throw() {
			const bool result = mTask.OnPostExecuteI();
			mWaitCondition.notify_all();
			return result;
		}

		inline TaskI::State GetState() const throw() {
			return mTask.GetState();
		}

		// Inherited from TaskCallbacks
		
		bool SOLAIRE_EXPORT_CALL Cancel() throw() override {
			const bool result = mTask.OnCancelI();
			mWaitCondition.notify_all();
			return result;
		}

		bool SOLAIRE_EXPORT_CALL Wait() throw() override {
			std::unique_lock<std::mutex> lock(mLock);
			mWaitCondition.wait(lock);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t aMilliseconds) throw() override {
			std::unique_lock<std::mutex> lock(mLock);
			return mWaitCondition.wait_for(lock, std::chrono::milliseconds(aMilliseconds)) == std::cv_status::no_timeout;
		}
	};

}

#endif