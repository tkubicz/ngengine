/* 
 * File:   Process.hpp
 * Author: tku
 *
 * Created on 7 August 2015, 14:49
 */

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "NGE/Core/Core.hpp"
#include <memory>

namespace NGE {
	namespace Core {
		class Process;
		typedef std::shared_ptr<Process> StrongProcessPtr;
		typedef std::weak_ptr<Process> WeakProcessPtr;

		/**
		 * @brief Process class represents single process in the engine.
		 * 
		 * Processes are ended by one of three methods: Success, Failure or Aborted.
		 * - Success means the process completed successfully. If the process has a child, it will be
		 * attached to process manager.
		 * - Failure means the process started but failed in some way. If the process has a child,
		 * it will be aborted.
		 * - Aborted processes are processes that are cancelled while not submitted to the process manager.
		 * Depending on the circumstances, they may or may not have gotten an OnInit() call. For example,
		 * a process can spawn another process and call AttachChild() on itself. If the new process fails,
		 * the child will get an OnAbort() call on it, event though its status is @link State::RUNNING.
		 */
		class Process {
			friend class ProcessManager;

		  public:

			/**
			 * Available process states.
			 */
			enum State {
				// Processes that are neither dead nor alive.
				/** Create but not running. */
				UNINITIALISED = 0,
				/** Removed from the process list but not destroyed; this can happen when a process that is already running is parent for another process. */
				REMOVED,

				// Living processes.
				/** Initialised and running. */
				RUNNING,
				/** Initialised but paused. */
				PAUSED,

				// Dead processes.

				/** Completed successfully. */
				SUCCEEDED,
				/** Failed to compete. */
				FAILED,
				/** Aborted, may not have started. */
				ABORTED
			};

		  private:
			/**
			 * The current state of the process.
			 */
			State state;

			/**
			 * The child process, if any.
			 */
			StrongProcessPtr child;

		  public:
			/**
			 * Default constructor.
			 */
			Process();

			/**
			 * Destructor.
			 */
			virtual ~Process();

		  protected:

			/**
			 * Called during the first update. Responsible for setting the initial
			 * state (typically @link State::RUNNING @endlink).
			 */
			virtual void OnInit();

			/**
			 * Called every frame.
			 * TOOD: This method should be pure virtual (abstract) but it isn't
			 * because we can't register abstract classes in Lua. Think how to change it.
			 * @param deltaTime time passed from the last frame.
			 */
			virtual void OnUpdate(unsigned int deltaTime) { }

			/**
			 * Called if the process succeeds.
			 */
			virtual void OnSuccess();

			/**
			 * Called if the process fails.
			 */
			virtual void OnFail();

			/**
			 * Called if the process is aborted.
			 */
			virtual void OnAbort();

		  public:

			/**
			 * End the process with @link State::SUCCEEDED
			 */
			virtual void Succeed();

			/** End the process with @link State::FAILED
			 * 
			 */
			virtual void Fail();

			/**
			 * Pause the process and set state to @link State::PAUSED
			 */
			virtual void Pause();

			/**
			 * Run the process.
			 */
			virtual void Run();

			/**
			 * Get the current state of the process.
			 * @return Current process state as @link #State
			 */
			State GetState() const;

			/**
			 * Check if the process is alive. It is alive if
			 * the state equals @link State::RUNNING @endlink or @link State::PAUSED @endlink.
			 * @return true if process is alive, otherwise false.
			 */
			virtual bool IsAlive() const;

			/**
			 * Check if the process is dead. It is dead if
			 * the state equals @link State::SUCEEDED @endlink, @link State::FAILED or @endlink
			 * @link State::ABORTED @endlink.
			 * @return true if process is dead, otherwise false.
			 */
			virtual bool IsDead() const;

			/**
			 * Check if the process is remove.
			 * @return  true if process was removed, otherwise false.
			 */
			virtual bool IsRemoved() const;

			/**
			 * Check if the process is paused.
			 * @return true if process is paused, otherwise false.
			 */
			virtual bool IsPaused() const;

			/**
			 * Attach child process to this process.
			 * @param child Strong pointer to the child process.
			 */
			void AttachChild(StrongProcessPtr child);

			/**
			 * Release ownership of the child process.
			 * @return Strong pointer to the child process.
			 */
			StrongProcessPtr RemoveChild();

			/**
			 * Get the pointer to the child process but do not release
			 * ownership.
			 * @return Strong pointer to the child process.
			 */
			StrongProcessPtr PeekChild();

		  protected:

			/**
			 * Set the new state of the process. This method is private
			 * as it should not be invoked directly.
			 * @param state New state to be set.
			 */
			void SetState(State state);
		};
	}
}

#endif /* PROCESS_HPP */