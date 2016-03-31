/* 
 * File:   Timing.h
 * Author: tku
 *
 * Created on 21 May 2012, 13:33
 */

#ifndef TIMING_HPP
#define TIMING_HPP

#include <chrono>
#include <array>

namespace NGE {
	namespace Tools {

		class Timing {
		  private:

			/**
			 * Time buffer to keep formatted date.
			 */
			char* timeBuffer;

			/**
			 * Size of the time buffer.
			 */
			unsigned short timeBufferSize;

			unsigned frameNumber;
			double lastFrameTimestamp;
			double lastFrameDuration;
			double averageFrameDuration;
			float fps;

			/**
			 * Flag that indicates if the timer is initialised.
			 */
			bool initialised;

			/**
			 * Flag that indicates if the timer is paused or not.
			 */
			bool paused;
		  private:

			Timing();
			~Timing();
			Timing(const Timing&);
			Timing& operator=(const Timing&);

			void SetInternalFieldsToDefault();

		  public:

			static Timing& GetInstance();

			void Update();
			void Initialize();
			void Deinitialize();

			std::string GetCurrentTimeInFormat(const std::string& format = "%Y-%m-%d %H:%M:%S.%f");
			std::string GetTimeInFormat(const std::chrono::milliseconds& milliseconds, const std::string& format = "%Y-%m-%d %H:%M:%S.%f");
			std::chrono::milliseconds GetCurrentTimeInMs();

			double GetLastFrameDuration() const;
			double GetAverageFrameDuration() const;
		};
	}
}

#endif /* TIMING_HPP */

