/* 
 * File:   NewLogger.hpp
 * Author: tku
 *
 * Created on 15 marca 2016, 02:55
 */

#ifndef NEWLOGGER_HPP
#define NEWLOGGER_HPP

#include <list>
#include <mutex>
#include <iomanip>
#include <cppformat/format.h>
#include <boost/format.hpp>

/**
 * TODO: Refactor this! This is just an example how a new logging system should looks
 * like.
 */
namespace log_2 {

    class tracer {
      private:
        std::ostream& out;
        boost::format formatter;

      public:

        tracer(std::ostream& out, const char* format, const char* type, char const* file, int line, const char* function) : out(out) {
            out << type << " - " << file << ":" << line << ":" << function << ": ";
            formatter = boost::format(format);
        }

        tracer(std::ostream& out, const char* format, const char* type) : out(out) {
            out << type << ": ";
            formatter = boost::format(format);
        }

        ~tracer() {
            out << formatter;
            out << std::endl;
        }

        template <typename TF, typename ... TR> void write(TF const& f, TR const& ... rest) {
            formatter % f;
            write(rest...);
        }

        template <typename TF> void write(TF const& f) {
            formatter % f;
        }

        void write() {
            // empty param
        }
    };

}

namespace NGE {
    namespace Tools {

        class NewLogger {
          public:

            enum class LOG_LEVEL {
                TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL_ERROR
            };

          private:

            /**
             * Holds currently accumulated logs. This structure is cleared
             * after flushing.
             */
            std::list<std::string> logStrings;

            /**
             * Path to the log file.
             */
            std::string logFilePath;

            /**
             * Flag that indicates that we are logging to the file.
             */
            bool loggingToFileEnabled;

            /**
             * Flag that indicates that we are logging to standard output.
             */
            bool loggingtoStdOutEnabled;

            /**
             * Current logging level.
             */
            LOG_LEVEL logLevel;

            /**
             * Mutex to ensure that write and read operations are thread-safe.
             */
            std::mutex mutex;

            fmt::MemoryWriter memoryWriter;

          private:

            /**
             * Constructor that sets default values for fields. It is private,
             * because this class is a singleton. Use GetInstance() to obtain
             * instance of this class.
             */
            NewLogger() {
                logLevel = LOG_LEVEL::DEBUG;
                loggingToFileEnabled = false;
                loggingtoStdOutEnabled = true;
            }

            std::string GetCurrentTime() {
                std::time_t currentTime = std::time(NULL);
                char mbstr[100];
                if (std::strftime(mbstr, sizeof (mbstr), "%A %c", std::localtime(&currentTime))) {
                    return std::string(mbstr);
                }
                return NULL;
            }

          public:

            static NewLogger& GetInstance() {
                static NewLogger instance;
                return instance;
            }

            template<typename... Args> void WriteLog(const std::string& format, Args&&... arguments) {
                std::cout << "WriteLog Test: " << GetCurrentTime() << " " << fmt::format(format, std::forward<Args>(arguments)...) << std::endl;
            }
        };
    }
}

#define test_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, __VA_ARGS__);

#define DEBUG

#ifdef DEBUG
#define LOGDEBUG(format, ...) log_2::tracer(std::cout, format, "DEBUG", __FILE__, __LINE__, __PRETTY_FUNCTION__).write(__VA_ARGS__);
#define LOGINFO(format, ...) log_2::tracer(std::cout, format, "INFO", __FILE__, __LINE__, __PRETTY_FUNCTION__).write(__VA_ARGS__);
#define LOGWARN(format, ...) log_2::tracer(std::cout, format, "WARNING", __FILE__, __LINE__, __PRETTY_FUNCTION__).write(__VA_ARGS__);
#define LOGERROR(format, ...) log_2::tracer(std::cout, format, "ERROR", __FILE__, __LINE__, __PRETTY_FUNCTION__).write(__VA_ARGS__);
#define LOGCRITICAL(format, ...) log_2::tracer(std::cout, format, "CRITICAL ERROR", __FILE__, __LINE__, __PRETTY_FUNCTION__).write(__VA_ARGS__);
#else
#define LOGDEBUG(format, ...) log_2::tracer(std::cout, format, "DEBUG").write(__VA_ARGS__);
#define LOGINFO(format, ...) log_2::tracer(std::cout, format, "INFO").write(__VA_ARGS__);
#define LOGWARN(format, ...) log_2::tracer(std::cout, format, "WARNING").write(__VA_ARGS__);
#define LOGERROR(format, ...) log_2::tracer(std::cout, format, "ERROR").write(__VA_ARGS__);
#define LOGCRITICAL(format, ...) log_2::tracer(std::cout, format, "CRITICAL ERROR").write(__VA_ARGS__);
#endif

#endif /* NEWLOGGER_HPP */

