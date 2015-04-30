/* 
 * File:   Timing.h
 * Author: tku
 *
 * Created on 21 maj 2012, 13:33
 */

#ifndef TIMING_HPP
#define	TIMING_HPP

namespace NGE
{
    namespace Tools
    {

        class Timing
        {
        public:
            unsigned frameNumber;
            double lastFrameTimestamp;
            double lastFrameDuration;
            //unsigned long lastFrameClockstamp;
            //unsigned long lastFrameClockTicks;

            bool isPaused;

            double averageFrameDuration;
            float fps;

            static Timing& Get();
            static void Update();
            static void Initialize();
            static void Deinitialize();

            static unsigned GetTime();
            //static unsigned long GetClock();

        private:

            Timing() { }

            Timing(const Timing&) { }

            Timing& operator =(const Timing&) { return *this; }
        };
    }
}

#endif	/* TIMING_HPP */

