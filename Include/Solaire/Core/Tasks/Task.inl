#ifndef SOLAIRE_CORE_TASK_INL
#define SOLAIRE_CORE_TASK_INL

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
	\file Task.inl
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 2.0
	\date
	Created			: 30th September 2015
	Last Modified	: 30th September 2015
*/

#include "Task.hpp"

namespace Solaire{ namespace Core{

    // Task

   bool Task::StateTransition(const StateCondition aCondition, const StateAction aAction, const State aTransition, const std::string& aName){
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

    bool Task::PreExecuteCondition() const{
        return mState == STATE_SCHEDULED && ! CaughtException();
    }

    bool Task::ExecuteCondition() const{
        return mState == STATE_PRE_EXECUTION && ! CaughtException();
    }

    bool Task::PostExecuteCondition() const{
        return mState == STATE_EXECUTION && ! CaughtException();
    }

    bool Task::PauseCondition() const{
        return mState == STATE_EXECUTION && ! CaughtException();
    }

    bool Task::ResumeCondition() const{
        return mState == STATE_PAUSED && ! CaughtException();
    }

    bool Task::CancelCondition() const{
        return mState != STATE_COMPLETE && mState != STATE_CANCELED && ! CaughtException();
    }

    bool Task::ResetCondition() const{
        return mState == STATE_CANCELED || mState == STATE_COMPLETE;
    }

    void Task::Schedule(TaskManager& aManager){
        mLock.lock();
        mManager = &aManager;
    }

    void Task::OnPreExecuteInternal(){
        OnPreExecute();
    }

    void Task::OnExecuteInternal(){
        OnExecute(mPauseLocation);
        mPauseLocation = 0;
    }

    void Task::OnPostExecuteInternal(){
        OnPostExecute();
        OnEndState();
    }

    void Task::OnPauseInternal(){
        OnPause();
    }

    void Task::OnResumeInternal(){
        OnResume(mPauseLocation);
    }

    void Task::OnResetInternal(){
        mPauseLocation = 0;
        mException = nullptr;
        mLock.unlock();
        OnReset();
    }

    void Task::OnCancelInternal(){
        OnCanceled();
    }

    void Task::OnEndState(){
        mLock.unlock();
        mManager = nullptr;
        for(void* i : mProgressList){
            DestroyProgress(i);
        }
    }

    bool Task::PreExecute(){
        return StateTransition(
            &Task::PreExecuteCondition,
            &Task::OnPreExecuteInternal,
            STATE_PRE_EXECUTION,
            "pre-execution"
        );
    }

    bool Task::Execute(){
        return StateTransition(
            &Task::ExecuteCondition,
            &Task::OnExecuteInternal,
            STATE_EXECUTION,
            "execution"
        );
    }

    bool Task::PostExecute(){
        return StateTransition(
            &Task::PostExecuteCondition,
            &Task::OnPostExecuteInternal,
            STATE_POST_EXECUTION,
            "post-execution"
        );
    }

    bool Task::Resume(){
        return StateTransition(
            &Task::ResumeCondition,
            &Task::OnResumeInternal,
            STATE_PRE_EXECUTION,
            "resumed"
        );
    }

    void Task::HandleProgress(){
        for(void* i : mProgressList){
            OnProgress(i);
            DestroyProgress(i);
        }
        mProgressList.Clear();
    }

    void Task::SendProgress(void* const aProgress){
        Task& lock = *this;
        solaire_synchronized(lock,
            mProgressList.PushBack(aProgress);
        )
    }

    void Task::Task::lock(){
        GetManager().mLock.lock();
    }

    void Task::unlock(){
        GetManager().mLock.unlock();
    }

    bool Task::try_lock(){
        return GetManager().mLock.try_lock();
    }

    bool Task::Pause(const uint16_t aLocation){
        mPauseLocation = aLocation;
        return StateTransition(
            &Task::PauseCondition,
            &Task::OnPauseInternal,
            STATE_PAUSED,
            "paused"
        );
    }

    Task::Task() :
        mManager(nullptr),
        mPauseLocation(0)
    {}

    Task::~Task(){
        for(void* i : mProgressList){
            //! \TODO Uncomment lines
            //OnProgress(i);
            //DestroyProgress(i);
        }
    }

    Task::State Task::GetState() const{
        return mState;
    }

    bool Task::Cancel(){
        return StateTransition(
            &Task::CancelCondition,
            &Task::OnCancelInternal,
            STATE_CANCELED,
            "canceled"
        );
    }

    bool Task::Reset(){
        return StateTransition(
            &Task::ResetCondition,
            &Task::OnResetInternal,
            STATE_INITIALISED,
            "reset"
        );
    }

    bool Task::CaughtException() const{
        return mException != nullptr;
    }

    void Task::RethrowException(){
        std::rethrow_exception(mException);
    }

    TaskManager& Task::GetManager(){
        if(mManager == nullptr) throw std::runtime_error("This task is not currently registered with a TaskManager");
        return *mManager;
    }

    const TaskManager& Task::GetManager() const{
        if(mManager == nullptr) throw std::runtime_error("This task is not currently registered with a TaskManager");
        return *mManager;
    }

    void Task::Wait() const{
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

    bool Task::TryWait() const{
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

    // TaskManager

    TaskManager::ThreadID TaskManager::GetThreadID() const{
        #ifdef SOLAIRE_DISABLE_MULTITHREADING
            return 0;
        #else
            return std::this_thread::get_id();
        #endif
    }

    bool TaskManager::Execute(){
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
        if(task->GetState() == Task::STATE_CANCELED) goto TASK_CANCELED;


        // If the task is paused, resume it
        if(task->GetState() == Task::STATE_PAUSED){
            if(! task->Resume()) goto TASK_FAIL;
        }

        // Execute the task
        if(! task->Execute()) goto TASK_FAIL;

        // Check which state the task is in after execution
        switch(task->GetState()){
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

    TaskManager::TaskManager(Allocator& aAllocator) :
        mInitialiseTasks(128, aAllocator),
        mPreExecuteTasks(128, aAllocator),
        mPostExecuteTasks(128, aAllocator),
        mCancelTasks(128, aAllocator)
    {}

    TaskManager::~TaskManager(){
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

    bool TaskManager::Schedule(Task& aTask){
        if(aTask.GetState() != Task::STATE_INITIALISED) return false;
        aTask.mState = Task::STATE_SCHEDULED;

        solaire_synchronized(mLock,
            mInitialiseTasks.PushBack(&aTask);
        )
        return true;
    }

    void TaskManager::Update(){
        solaire_synchronized(mLock,
            // OnPreExecute
            for(Task* task : mInitialiseTasks){
                task->Schedule(*this);
                if(task->GetState() == Task::STATE_CANCELED){
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
                if(task->GetState() == Task::STATE_CANCELED){
                    mCancelTasks.PushBack(task);
                }else if(task->GetState() != Task::STATE_PAUSED){
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

    size_t TaskManager::TaskCount() const{
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

    void TaskManager::Wait(const size_t aSleepTime){
        while(TaskCount() > 0){
            Update();
            #ifndef SOLAIRE_DISABLE_MULTITHREADING
                std::this_thread::sleep_for(std::chrono::milliseconds(aSleepTime));
            #endif
        }
    }

    /*template<class T>
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
    };*/
}}


#endif
