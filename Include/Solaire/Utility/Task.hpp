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
	\version 2.0
	\date
	Created			: 11th September 2015
	Last Modified	: 24th September 2015
*/

#include <map>
#ifndef SOLAIRE_DISABLE_MULTITHREADING
    #include <thread>
    #include <mutex>
#else
    #include "DummyLock.hpp"
#endif
#include "..\Core\Macros.hpp"
#include "..\Core\DataStructures/DynamicArray.hpp"

namespace Solaire{ namespace Utility{

    class TaskManager;

    class Task{
    public:
        enum State : uint8_t {
			STATE_INITIALISED,
			STATE_SCHEDULED,
			STATE_PRE_EXECUTION,
			STATE_PAUSED,
			STATE_EXECUTION,
			STATE_POST_EXECUTION,
			STATE_CANCELED,
			STATE_COMPLETE
		};
        friend TaskManager;
    private:
        #ifdef SOLAIRE_DISABLE_MULTITHREADING
            typedef DummyLock Mutex;
        #else
            typedef std::mutex Mutex;
        #endif

        Core::DynamicArray<void*> mProgressList;
        TaskManager* mManager;
        std::exception_ptr mException;
        mutable Mutex mLock;
        uint16_t mPauseLocation;
        State mState;

        typedef bool(Task:: *StateCondition)() const;
        typedef void(Task:: *StateAction)();

        bool StateTransition(const StateCondition aCondition, const StateAction aAction, const State aTransition, const std::string& aName){
            if((this->*aCondition)()){
                try{
                    (this->*aAction)();
                    mState = aTransition;
                    return true;
                }catch(...){
                    mException = std::current_exception();
                    Cancel();
                }
            }else{
                mException = std::make_exception_ptr(std::runtime_error("Utility::Task : Can only enter " + aName));
            }
            return false;
        }

        bool PreExecuteCondition() const{
            return mState == STATE_SCHEDULED && ! CaughtException();
        }

        bool ExecuteCondition() const{
            return mState == STATE_PRE_EXECUTION && ! CaughtException();
        }

        bool PostExecuteCondition() const{
            return mState == STATE_EXECUTION && ! CaughtException();
        }

        bool PauseCondition() const{
            return mState == STATE_EXECUTION && ! CaughtException();
        }

        bool ResumeCondition() const{
            return mState == STATE_PAUSED && ! CaughtException();
        }

        bool CancelCondition() const{
            return mState != STATE_COMPLETE && mState != STATE_CANCELED && ! CaughtException();
        }

        bool ResetCondition() const{
            return mState == STATE_CANCELED || mState == STATE_COMPLETE;
        }

        void Schedule(TaskManager& aManager){
            mLock.lock();
            mManager = &aManager;
        }

        void OnPreExecuteInternal(){
            OnPreExecute();
        }

        void OnExecuteInternal(){
            OnExecute(mPauseLocation);
            mPauseLocation = 0;
        }

        void OnPostExecuteInternal(){
            OnPostExecute();
            OnEndState();
        }

        void OnPauseInternal(){
            OnPause();
        }

        void OnResumeInternal(){
            OnResume(mPauseLocation);
        }

        void OnResetInternal(){
            mPauseLocation = 0;
            mException = nullptr;
            mLock.unlock();
            OnReset();
        }

        void OnCancelInternal(){
            OnCanceled();
        }

        void OnEndState(){
            mLock.unlock();
            mManager = nullptr;
            for(void* i : mProgressList){
                DestroyProgress(i);
            }
        }

        bool PreExecute(){
            return StateTransition(
                &Task::PreExecuteCondition,
                &Task::OnPreExecuteInternal,
                STATE_PRE_EXECUTION,
                "pre-execution"
            );
        }

        bool Execute(){
            return StateTransition(
                &Task::ExecuteCondition,
                &Task::OnExecuteInternal,
                STATE_EXECUTION,
                "execution"
            );
        }

        bool PostExecute(){
            return StateTransition(
                &Task::PostExecuteCondition,
                &Task::OnPostExecuteInternal,
                STATE_POST_EXECUTION,
                "post-execution"
            );
        }

        bool Resume(){
            return StateTransition(
                &Task::ResumeCondition,
                &Task::OnResumeInternal,
                STATE_PRE_EXECUTION,
                "resumed"
            );
        }

        void HandleProgress(){
            for(void* i : mProgressList){
                OnProgress(i);
                DestroyProgress(i);
            }
            mProgressList.Clear();
        }

    protected:
        virtual void OnPreExecute() = 0;
        virtual void OnExecute(const uint16_t aLocation = 0) = 0;
        virtual void OnPostExecute() = 0;
        virtual void OnPause() = 0;
        virtual void OnResume(const uint16_t aLocation) = 0;
        virtual void OnCanceled() = 0;
        virtual void OnReset() = 0;

        virtual void DestroyProgress(void* const aProgress) = 0;
        virtual void OnProgress(void* const aProgress) = 0;

        void SendProgress(void* const aProgress){
            Task& lock = *this;
            solaire_synchronized(lock,
                mProgressList.PushBack(aProgress);
            )
        }

        void lock();
        void unlock();
        bool try_lock();

        bool Pause(const uint16_t aLocation){
            mPauseLocation = aLocation;
            return StateTransition(
                &Task::PauseCondition,
                &Task::OnPauseInternal,
                STATE_PAUSED,
                "paused"
            );
        }
    public:
        Task() :
            mManager(nullptr),
            mPauseLocation(0)
        {}

        virtual ~Task(){
            for(void* i : mProgressList){
                OnProgress(i);
                DestroyProgress(i);
            }
        }

        State State() const{
            return mState;
        }

        bool Cancel(){
            return StateTransition(
                &Task::CancelCondition,
                &Task::OnCancelInternal,
                STATE_CANCELED,
                "canceled"
            );
        }

        bool Reset(){
            return StateTransition(
                &Task::ResetCondition,
                &Task::OnResetInternal,
                STATE_INITIALISED,
                "reset"
            );
        }

        bool CaughtException() const{
            return mException != nullptr;
        }

        void RethrowException(){
            std::rethrow_exception(mException);
        }

        TaskManager& GetManager(){
            if(mManager == nullptr) throw std::runtime_error("This task is not currently registered with a TaskManager");
            return *mManager;
        }

        const TaskManager& GetManager() const{
            if(mManager == nullptr) throw std::runtime_error("This task is not currently registered with a TaskManager");
            return *mManager;
        }

        void Wait() const{
            switch(mState){
            case STATE_CANCELED:
            case STATE_COMPLETE:
            case STATE_INITIALISED:
                break;
            default:
                mLock.lock();
                mLock.unlock();
                break;
            }
        }

        bool TryWait() const{
            switch(mState){
            case STATE_CANCELED:
            case STATE_COMPLETE:
            case STATE_INITIALISED:
                break;
            default:
                if(mLock.try_lock()){
                    mLock.unlock();
                    return true;
                }else{
                    return false;
                }
            }
        }

        //! \TODO post progress
    };

    class TaskManager{
    private:
        TaskManager(const TaskManager&) = delete;
        TaskManager(TaskManager&&) = delete;
        TaskManager& operator=(const TaskManager&) = delete;
        TaskManager& operator=(TaskManager&&) = delete;

        #ifdef SOLAIRE_DISABLE_MULTITHREADING
            typedef uint32_t ThreadID;
            typedef DummyLock Mutex;
        #else
            typedef std::thread::id ThreadID;
            typedef std::mutex Mutex;
        #endif

        Core::DynamicArray<Task*> mInitialiseTasks;
        Core::DynamicArray<Task*> mPreExecuteTasks;
        Core::DynamicArray<Task*> mPostExecuteTasks;
        Core::DynamicArray<Task*> mCancelTasks;
        std::map<ThreadID, Task*> mExecutionTasks;

        ThreadID GetThreadID() const{
            #ifdef SOLAIRE_DISABLE_MULTITHREADING
                return 0;
            #else
                return std::this_thread::get_id();
            #endif
        }
    protected:
        Mutex mLock;

        bool Execute(){
            // Acquire task from manager
            Task* task = nullptr;
            solaire_synchronized(mLock,
                if(! mPreExecuteTasks.IsEmpty()){
                    task = mPreExecuteTasks.PopBack();
                    mExecutionTasks.emplace(GetThreadID(), task);
                }
            )

            // If a task could not be acquired
            if(task == nullptr) goto TASK_NULL;

            // Check if the task is canceled
            if(task->State() == Task::STATE_CANCELED) goto TASK_CANCELED;


            // If the task is paused, resume it
            if(task->State() == Task::STATE_PAUSED){
                if(! task->Resume()) goto TASK_FAIL;
            }

            // Execute the task
            if(! task->Execute()) goto TASK_FAIL;

            // Check which state the task is in after execution
            switch(task->State()){
            case Task::STATE_PAUSED:
                goto TASK_PAUSED;
            case Task::STATE_CANCELED:
                goto TASK_CANCELED;
            default:
                goto TASK_SUCCESS;
            }


            TASK_NULL:
            return false;

            TASK_CANCELED:
            solaire_synchronized(mLock,
                mCancelTasks.PushBack(task);
                mExecutionTasks.emplace(GetThreadID(), nullptr);
            )

            TASK_PAUSED:
            solaire_synchronized(mLock,
                mPreExecuteTasks.PushBack(task);
                mExecutionTasks.emplace(GetThreadID(), nullptr);
            )
            return true;

            TASK_SUCCESS:
            solaire_synchronized(mLock,
                mPostExecuteTasks.PushBack(task);
                mExecutionTasks.emplace(GetThreadID(), nullptr);
            )
            return true;

            TASK_FAIL:
            solaire_synchronized(mLock,
                mExecutionTasks.emplace(GetThreadID(), nullptr);
            )
            return true;

        }
    public:
        friend Task;

        TaskManager(Core::Allocator<Task*>& aAllocator) :
            mInitialiseTasks(128, aAllocator),
            mPreExecuteTasks(128, aAllocator),
            mPostExecuteTasks(128, aAllocator),
            mCancelTasks(128, aAllocator)
        {}

        virtual ~TaskManager(){
            solaire_synchronized(mLock,
                for(Task* task : mInitialiseTasks){
                    task->Cancel();
                    mCancelTasks.PushBack(task);
                }
                mInitialiseTasks.Clear();

                for(Task* task : mPreExecuteTasks){
                    task->Cancel();
                    mCancelTasks.PushBack(task);
                }
                mPreExecuteTasks.Clear();

                for(Task* task : mPostExecuteTasks){
                    task->Cancel();
                    mCancelTasks.PushBack(task);
                }
                mPostExecuteTasks.Clear();

                for(Task* task : mCancelTasks){
                    task->OnEndState();
                }
                mCancelTasks.Clear();
            )
        }

        virtual bool Schedule(Task& aTask){
            if(aTask.State() != Task::STATE_INITIALISED) return false;
            aTask.mState = Task::STATE_SCHEDULED;

            solaire_synchronized(mLock,
                mInitialiseTasks.PushBack(&aTask);
            )
            return true;
        }

        virtual void Update(){
            solaire_synchronized(mLock,
                // OnPreExecute
                for(Task* task : mInitialiseTasks){
                    task->Schedule(*this);
                    if(task->State() == Task::STATE_CANCELED){
                        mCancelTasks.PushBack(task);
                    }else if(task->PreExecute()){
                        mPreExecuteTasks.PushBack(task);
                    }
                }
                mInitialiseTasks.Clear();

                // OnProgress
                for(std::pair<ThreadID, Task*> task : mExecutionTasks){
                    task.second->HandleProgress();
                }

                // OnPostExecute
                for(Task* task : mPostExecuteTasks){
                    if(task->State() == Task::STATE_CANCELED){
                        mCancelTasks.PushBack(task);
                    }else if(task->State() != Task::STATE_PAUSED){
                        task->PostExecute();
                    }
                }
                mPostExecuteTasks.Clear();

                // OnCancel
                for(Task* task : mCancelTasks){
                    task->OnEndState();
                }
                mCancelTasks.Clear();
            )
        }

        virtual size_t ThreadCount() const = 0;

        size_t TaskCount() const{
            size_t count = 0;
            solaire_synchronized(mLock,
                for(const std::pair<ThreadID, Task*>& i : mExecutionTasks){
                    if(i.second != nullptr) ++count;
                }
                count += mInitialiseTasks.Size();
                count += mPreExecuteTasks.Size();
                count += mPostExecuteTasks.Size();
                count += mCancelTasks.Size();
            )
            return count;
        }

        virtual void Wait(const size_t aSleepTime = 33){
            while(TaskCount() > 0){
                Update();
                #ifndef SOLAIRE_DISABLE_MULTITHREADING
                    std::this_thread::sleep_for(std::chrono::milliseconds(aSleepTime));
                #endif
            }
        }
    };

    void Task::lock(){
        GetManager().mLock.lock();
    }

    void Task::unlock(){
        GetManager().mLock.lock();
    }

    bool Task::try_lock(){
        GetManager().mLock.lock();
    }

    template<class T>
    class ResultTask : public Task{
    private:
        T mResult;
    protected:
        virtual void OnPreExecute(T& aResult) = 0;
        virtual void OnExecute(T& aResult) = 0;
        virtual void OnPostExecute(T& aResult) = 0;
        virtual void OnCanceled(T& aResult) = 0;
        virtual void OnReset(T& aResult) = 0;
        virtual void OnPause(T& aResult) = 0;
        virtual void OnResume(const uint16_t aLocation, T& aResult) = 0;

        // Inherited from Task

        void OnReset() override{
            OnReset(mResult);
        }

        void OnPreExecute() override{
            OnPreExecute(mResult);
        }

        void OnExecute() override{
            OnExecute(mResult);
        }

        void OnPostExecute() override{
            OnPostExecute(mResult);
        }

        void OnCanceled() override{
            OnCanceled(mResult);
        }

        void OnPause() override{
            OnPause(mResult);
        }

        void OnResume(const uint16_t aLocation) override{
            OnResume(aLocation, mResult);
        }
    public:

        virtual ~ResultTask(){

        }

        const T& Get() const{
            if(CaughtException()) RethrowException();

            switch(State()){
            case STATE_INITIALISED:
                throw std::runtime_error("Cannot get result while in STATE_INITIALISED");
            case STATE_CANCELED:
                throw std::runtime_error("Cannot get result while in STATE_CANCELED");
            case STATE_COMPLETE:
                return mResult;
            default:
                Wait();
                return Get();
            }
        }
    };
}}


#endif
