/* 
 * File:   Timing.h
 * Author: tku
 *
 * Created on 21 maj 2012, 13:33
 */

#ifndef TIMING_HPP
#define TIMING_HPP

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
			bool isPaused;
			double averageFrameDuration;
			float fps;

		  private:

			Timing() {
				timeBufferSize = 26;
				timeBuffer = new char[timeBufferSize];
			}

			~Timing() {
				delete timeBuffer;
			}

			Timing(const Timing&) { }

			Timing& operator=(const Timing&) {
				return *this;
			}

		  public:

			static Timing& GetInstance() {
				static Timing instance;
				return instance;
			}

			void Update();
			void Initialize();
			void Deinitialize();

			unsigned GetTime();
			//static unsigned long GetClock();

			std::string GetCurrentTimeInFormat(const std::string& format = "%Y-%m-%d %H:%M:%S");

			double GetLastFrameDuration() const {
				return lastFrameDuration;
			}
		};
	}
}

#endif /* TIMING_HPP */

