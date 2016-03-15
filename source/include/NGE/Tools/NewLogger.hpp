/* 
 * File:   NewLogger.hpp
 * Author: tku
 *
 * Created on 15 marca 2016, 02:55
 */

#ifndef NEWLOGGER_HPP
#define NEWLOGGER_HPP

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

