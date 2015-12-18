#ifndef SOLAIRE_TASKI_HPP
#define SOLAIRE_TASKI_HPP

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
\file TaskI.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 6th December 2015
Last Modified	: 8th December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"
#include "..\Core\System.hpp"

namespace Solaire {

	class ThreadPool;
	class TaskCallbacks;

	template<const int INIT_MODE, const int PRE_MODE, const int EXE_MODE, const int POST_MODE>
	class TaskGroup;

	SOLAIRE_EXPORT_INTERFACE TaskI {
	public :
		template<const int, const int, const int, const int>
		friend class TaskGroup;

		friend ThreadPool;
		enum State : uint8_t{
			STATE_INITIALISED,
			STATE_PRE_EXECUTE,
			STATE_EXECUTE,
			STATE_PAUSED,
			STATE_POST_EXECUTE,
			STATE_CANCELED,
			STATE_COMPLETE
		};

		struct Configuration {
			uint64_t PauseTime;
			uint16_t PauseDuration;
			struct {
				uint8_t State : 3;
				uint8_t ExecutesOnWorker : 1;
				uint8_t IsPausable : 1;
				uint8_t IsReinitialisable : 1;
				uint8_t SkipPreExecute : 1;
				uint8_t SkipPostExecute : 1;
			};

			Configuration() :
				PauseTime(0),
				PauseDuration(0),
				State(STATE_INITIALISED),
				ExecutesOnWorker(1),
				IsPausable(1),
				IsReinitialisable(1),
				SkipPreExecute(0),
				SkipPostExecute(0)
			{}
		};
	protected :
		virtual void SOLAIRE_EXPORT_CALL NotifyWait() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnInitialise() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPreExecute() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnExecute() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPostExecute() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnPause() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnResume() throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL OnCancel() throw() = 0;

		virtual Configuration& SOLAIRE_EXPORT_CALL GetConfigurationRef() throw() = 0;

		SOLAIRE_FORCE_INLINE void SOLAIRE_DEFAULT_CALL Unpause() throw() {
			GetConfigurationRef().PauseDuration = 0;
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Initialise() throw() {
			Configuration& config = GetConfigurationRef();
			switch(config.State) {
			case STATE_INITIALISED:
				return true;
			case STATE_CANCELED:
			case STATE_COMPLETE:
				if(! config.IsReinitialisable) return false;
				config.PauseTime = 0;
				config.PauseDuration = 0;
				config.State = STATE_INITIALISED;
				return OnInitialise();
			default:
				return false;
			}
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL PreExecute() throw() {
			Configuration& config = GetConfigurationRef();
			if(config.State != STATE_INITIALISED || config.SkipPreExecute) return false;
			config.State = STATE_PRE_EXECUTE;
			return OnPreExecute();
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Execute() throw() {
			Configuration& config = GetConfigurationRef();
			if(config.State != STATE_PRE_EXECUTE) return false;
			config.State = STATE_EXECUTE;
			const bool result = OnExecute();
			if(config.State == STATE_EXECUTE) config.State = STATE_POST_EXECUTE;
			return result;
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL PostExecute() throw() {
			Configuration& config = GetConfigurationRef();
			if(config.State != STATE_POST_EXECUTE || config.SkipPostExecute) return false;
			const bool result = OnPostExecute();
			config.State = STATE_COMPLETE;
			NotifyWait();
			return result;
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Pause(const uint16_t aMilliseconds) throw() {
			Configuration& config = GetConfigurationRef(); 
			if(config.State != STATE_EXECUTE || ! config.IsPausable) return false;
			const bool result = OnPause();
			config.State = STATE_PAUSED;
			config.PauseTime = GetTimeMilliseconds();
			config.PauseDuration = aMilliseconds;
			return result;
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Resume() throw() {
			Configuration& config = GetConfigurationRef();
			if(config.State != STATE_PAUSED || ! config.IsPausable) return false;
			config.State = STATE_EXECUTE;
			return OnResume();
		}
	public:
		virtual Configuration SOLAIRE_EXPORT_CALL GetConfiguration() const throw() = 0;
		virtual SOLAIRE_EXPORT_CALL ~TaskI() throw(){}
		virtual bool SOLAIRE_EXPORT_CALL Wait() const throw() = 0;
		virtual bool SOLAIRE_EXPORT_CALL WaitFor(const uint32_t aMilliseconds) const throw() = 0;

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL Cancel() throw() {
			Configuration& config = GetConfigurationRef();
			if (config.State == STATE_CANCELED || config.State == STATE_COMPLETE) return false;
			const bool result = OnCancel();
			NotifyWait();
			return result;
		}

		SOLAIRE_FORCE_INLINE State SOLAIRE_DEFAULT_CALL GetState() const throw() {
			return static_cast<State>(GetConfiguration().State);
		}

		SOLAIRE_FORCE_INLINE bool SOLAIRE_DEFAULT_CALL ExecutesOnWorker() const throw() {
			return GetConfiguration().ExecutesOnWorker;
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetPauseTime() const throw() {
			return GetConfiguration().PauseTime;
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetPauseDuration() const throw() {
			return GetConfiguration().PauseDuration;
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_DEFAULT_CALL GetResumeTime() const throw() {
			const Configuration tmp = GetConfiguration();
			return tmp.PauseTime + tmp.PauseDuration;
		}
	};
}


#endif
