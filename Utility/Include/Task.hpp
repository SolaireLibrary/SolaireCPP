#ifndef SOLAIRE_UTILITY_TASK_HPP
#define SOLAIRE_UTILITY_TASK_HPP

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
	\file Task.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th September 2015
	Last Modified	: 11th September 2015
*/

#include <type_traits>
#include <atomic>

namespace Solaire{ namespace Utility{

    class Task;

    class TaskManager{
    protected:
        friend Task;

        virtual void SendProgress(Task* aTask, void* aProgress) = 0;
        virtual void Schedule(Task* aTask) = 0;
    public:
        virtual ~TaskManager(){

        }

		virtual bool CanUpdate() const = 0;
        virtual void Update() = 0;
    };

    class AsyncManager;

    class Task{
	public:
		enum State : uint8_t {
			NOT_SCHEDULED,
			PRE_EXECUTION,
			EXECUTING,
			POST_EXECUTION,
			CANCELED,
			EXECUTED
		};
    private:
        friend AsyncManager;

        std::atomic_bool mCanceled;
        TaskManager* mManager;
		std::atomic_uint8_t mState;

        Task(const Task&) = delete;
        Task(Task&&) = delete;
        Task& operator=(const Task&) = delete;
        Task& operator=(Task&&) = delete;
    protected:
        virtual void OnRecieveProgress(void* aProgress) = 0;

        void SendProgress(void* aProgress){
            mManager->SendProgress(this, aProgress);
        }

        bool HasBeenCancled() const{
            return mCanceled;
        }

		virtual void PreExecute() = 0;
		virtual void Execute() = 0;
		virtual void PostExecute() = 0;
    public:
        Task() :
            mCanceled(false),
            mManager(nullptr),
			mState(NOT_SCHEDULED)
        {

        }

        virtual ~Task(){

        }

		State GetState() const {
			return static_cast<State>(mState.load());
		}

		void Schedule(TaskManager& aManager){
			mManager = &aManager;
			mManager->Schedule(this);
		}

        void Cancel(){
            mCanceled = true;
        }

		void Wait() {
			//! \TODO Implement better Task.Wait

			auto IsDone = [&](){
				const State state = GetState();
				return state == CANCELED || state == EXECUTED;
			};

			while (!IsDone()) std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}

		bool WaitFor(const size_t aMilliseconds) {
			//! \TODO Implement better Task.WaitFor

			auto IsDone = [&]() {
				const State state = GetState();
				return state == CANCELED || state == EXECUTED;
			};

			if(IsDone()) return true;
			std::this_thread::sleep_for(std::chrono::milliseconds(aMilliseconds));
			return IsDone();
		}

		bool WaitUntil(const size_t aTimeMilliseconds) {
			//! \TODO Implement Task.WaitUntil
			throw std::runtime_error("Not implemented");
			// Return WaitFor(aTimeMilliseconds - currentTimeMilliseconds);
		}
    };

    template<typename RESULT, typename PROGRESS>
    class ReturnTask : public Task{
    public:
        static_assert(std::is_default_constructible<RESULT>::value, "Task.result_t must have a default constructor");
        static_assert(std::is_copy_assignable<RESULT>::value, "Task.result_t must have a copy assignment");
        static_assert(std::is_copy_constructible<PROGRESS>::value, "Task.progress_t must have a copy constructor");

        typedef RESULT result_t;
        typedef PROGRESS progress_t;
    private:
        result_t mResult;
    protected:
        virtual result_t ExecuteForResult() = 0;
		virtual void PostExecute(result_t& aResult) = 0;
        virtual void OnRecieveProgress(progress_t& aProgress) = 0;


        void SendProgress(progress_t&& aProgress){
            progress_t* tmp = new progress_t(std::move(aProgress));
            Task::SendProgress(tmp);
        }

        // Inherited from Task

        void Execute() override{
            mResult = ExecuteForResult();
        }

        void OnRecieveProgress(void* aProgress) override{
            progress_t* ptr = static_cast<progress_t*>(aProgress);
            OnRecieveProgress(*ptr);
            delete ptr;
        }

		void PostExecute() override{
			PostExecute(mResult);
		}
    public:
        ReturnTask()
        {

        }

        virtual ~ReturnTask(){

        }

		result_t Get(){
			if(GetState() != EXECUTED) throw std::runtime_error("Can only get result after a Task has successfully executed");
			return mResult;
		}
    };

}}


#endif